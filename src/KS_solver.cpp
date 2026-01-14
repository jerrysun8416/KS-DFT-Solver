    // g++ -O2 *.cpp -o *.exe
    // This setup is for H ~ C only.
    // ------------------------------------------------------
    // Problems:
    // 1. Enl is not very accurate. Improve solve_schrodinger() algorithm.
    // ------------------------------------------------------
    // Notes on parameters:
    // 1. When tweaking parameters, for recommend value search: \\@
    // 2. Changing `rmax` and `unl[N-2]` will make tremendous difference.
    // 3. Change E_converge in main() and `tol` in solve_schrodinger() if necessary to end loop.
    // ------------------------------------------------------
    // v5:
    // 1. Fixed U_Hartree dumb incorrect formula problem.
    // 2. Purged loop to find converged 1S 2S orbitals for H He Li Be.
    // ------------------------------------------------------
    // v6:
    // 1. Cleaned up unnecessary algorithms except Passiverefinement Numerov for Hartree_Numerov() and Solve_Schrodinger().
    // 2. Checked error_code of Solve_Schrodinger() in main() loop.
    // ------------------------------------------------------
    // v7:
    // Log the grid:Updated Numerov formula on ununiform grid FAILED. It works for even grid.
    // Notes on algorithm:
    // 1. Current Numerov formula is not accurate: y'[h(n) - h(n-1)] and y'''[h(n)^3 - h(n-1)^3] cannot be ignored. Lead to inaccuracy near r = 0. Where y' and y''' are large.
    // 2. Cannot apply Runge–Kutta method, due to unknown analytical density n(x). Where n(x + h(n)/2) is required.
    // ------------------------------------------------------
    // v8: 
    // Leave v7 problems. Improved the precision.
    // Build r(x) from uniform x, recover U(r) from U(x)?
    // Currently only works for H ~ Li. Hartree: Newton; Schrodinger: oscillation.
    // ------------------------------------------------------
    // v9:
    // Newton is required for solving Hartree.
    // Avoid oscillation: half density update.
    // B atom is not accurate. N is not solvable. somehow E_difference is changing periodically.
    // ------------------------------------------------------
    // v10:
    // Log the grid -- Finally worked!
    // x[i] = log_min + (log_step * i) = x_min + (dx * i)       r_grid[i] = exp(x[i])
    // Applied correct formula w. Numerov on log grid: solve U_Hartree; solve unl
    // To do next: 1. ED. 2. MPI for solving each orbital K-S Eqn.
    #include <iostream>
    #include <fstream>
    #include <cmath>
    #include <vector>
    #include <iomanip>
    #include <numeric>
    #include <string>
    #include <sstream>
    #include <algorithm>
    #include <tuple>
    #include "atom_database.h"
    #include <complex>
    #include <map>
    #include <Eigen/Dense>



    // Constant parameters
    const double PI = 3.141592653589793;
    const double E_numb = 2.718281828459045;
    // Correlation energy parameters
    const double A = 0.0621814;
    const double x0 = -0.10498;
    const double b = 3.72744;
    const double c = 12.9352;
    const double Q = 6.151991;
    // Poisson parameters
    const double rmin = 1E-12; //@ 1E-10 < Mg
    const double rmax = 30.; //@ 12 < N 20 < Ne
    const int Nx = 20000; //@ 8000 < N
    // const double dx = rmax / static_cast<double>(Nx);
    // Schrodinger Loop iteration config
    const int Iter_max = 100;
    const double E_converge = 1E-5; // must greater than U_Hartree tol and Schrodinger tol
    const double density_smear_cutoff = 1E-20;
    // Grid parameters
    const double log_min = std::log(rmin);
    const double log_max = std::log(rmax);
    const double log_step = (log_max - log_min) / static_cast<double>(Nx);
    // x[i] expression: log_min + (log_step * i) = x_min + (dx * i)

    //Initialize atomic config
    struct AtomData {
        const int Ntot;
        const std::vector<OrbitalStruct> orbitals;
        AtomData(int N, const std::vector<OrbitalStruct>& o)
            : Ntot(N), orbitals(o) {}
    };
    inline std::pair<AtomData, std::string> Select_Atom() { //Pass config into Atom_config
        std::string input;
        std::cout << "Enter atom name (H ~ Ca): ";
        std::cin >> input;
        auto it = AtomDB.find(input);
        if (it == AtomDB.end()) {
            std::cerr << "Invalid atom name.\n";
            std::exit(1);
        }
        else {
            std::cout << "Selected atom: " << input << "\t\tNtot = " << it -> second.first<< std::endl;
        }
        return { AtomData(it->second.first, it->second.second), input };
    };
    //---------------------------------------------------
    //Initialize grid
    struct InitializeStruct {
        std::vector<double> grid;
        std::vector<double> density;
    };
    InitializeStruct Initialize_n(double Z_nucleus, int Ntot){
        // uniform grid ---------------------------------------------------
        std::vector<double> r_grid(Nx + 1);
        double dx = rmax / static_cast<double>(Nx);
        for(std::size_t i = 0; i < r_grid.size(); ++i){
            r_grid[i] = i * dx;
        }
        r_grid[0] = 1E-8; // Make huge difference 1E-8 is required dont touch.
        // exponential ---------------------------------------------------
        // std::vector<double> r_grid(Nx + 1); // Choose log10 for 10 is a integer
        // for(std::size_t i = 0; i < r_grid.size(); ++i){
        //     r_grid[i] = std::pow(E_numb, log_min + (log_step * i));
        // }
        //---------------------------------------------------
        std::vector<double> density(Nx + 1);
        for (std::size_t i = 0; i < density.size(); ++i)
        {
            density[i] = Z_nucleus * Z_nucleus * std::exp(-1. * Z_nucleus * r_grid[i]); // Better than T-F density.
        }
        // Normalize density
        double density_integrate = 0.;
        double f_iminus1, f_i, f_iplus1;
        for (std::size_t i = 1; i < r_grid.size(); i += 2)
        {
            if (i == 1)
            {
                f_iminus1 = density[i - 1] * 4. * PI * r_grid[i - 1] * r_grid[i - 1];
            }
            else
            {
                f_iminus1 = f_iplus1;
            }
            f_i = density[i] * 4. * PI * r_grid[i] * r_grid[i];
            f_iplus1 = density[i + 1] * 4. * PI * r_grid[i + 1] * r_grid[i + 1];
            density_integrate += (f_iminus1 + 4. * f_i + f_iplus1) * (r_grid[i + 1] - r_grid[i - 1]) / 6.;
        }
        double factor = static_cast<double>(Ntot) / density_integrate;
        std::transform(density.begin(), density.end(), density.begin(), [factor](double x)
                    { return x * factor; }); // Better Normalized
        return {r_grid, density};
    }
    //---------------------------------------------------
    // Initialize basis w. G vector
    struct Gvector{
        int nx, ny, nz;
        double Gx, Gy, Gz, G2;
    };
    auto Construct_G_basis(double Ecutoff, double Lx, double Ly, double Lz){
        std::vector<Gvector> G_basis;
        int nx, ny, nz;
        int nx_max = std::ceil(Lx * std::sqrt(2. * Ecutoff) / (2. * PI));
        int ny_max = std::ceil(Ly * std::sqrt(2. * Ecutoff) / (2. * PI));
        int nz_max = std::ceil(Lz * std::sqrt(2. * Ecutoff) / (2. * PI));
        for(nx = -1*nx_max; nx < nx_max+1; ++nx){
            for(ny = -1*ny_max; ny < ny_max+1; ++ny){
                for(nz = -1*nz_max; nz < nz_max+1; ++nz){
                    double Gx = 2*PI*nx/Lx, Gy = 2*PI*ny/Ly, Gz = 2*PI*nz/Lz;
                    double G2 = Gx*Gx + Gy*Gy + Gz*Gz;
                    if(G2 <= 2. * Ecutoff){
                        G_basis.push_back({nx, ny, nz,Gx, Gy, Gz, G2});
                    }
        }
        }
        }
        return G_basis;
    }



    void Write_wavefunction(int n, int l, int nodes, const std::vector<double>& unl, const std::vector<double>& r_grid, std::string atom_name = "test") {
        std::ostringstream filename_stream;
        filename_stream << atom_name << "_n_" << n << "_l_" << l << ".dat";
        std::string filename = filename_stream.str();
        std::ofstream fout(filename);
        if (!fout) {
            std::cerr << "Error: Cannot open file for writing! filename = " << filename << "\n";
            return;
        }
        else {
            std::cout << "Done: Wavefunction file is written. filename = " << filename << std::endl;
        }
        for (std::size_t i = 0; i < r_grid.size(); ++i) {
            fout << r_grid[i] << " " << unl[i] << "\n";
        }
        fout.close();
    }

    void Write_xy(const std::vector<double>& x_lst, const std::vector<double>& y_lst, std::string title = "test") {
        std::ostringstream filename_stream;
        filename_stream << title << ".dat";
        std::string filename = filename_stream.str();
        std::ofstream fout(filename);
        if (!fout) {
            std::cerr << "Error: Cannot open file for writing! filename = " << filename << "\n";
            return;
        }
        else {
            std::cout << "Done: File is written. filename = " << filename << std::endl;
        }
        for (std::size_t i = 0; i < x_lst.size(); ++i) {
            fout << x_lst[i] << " " << y_lst[i] << "\n";
        }
        fout.close();
        // auto max_it = std::max_element(y_lst.begin(), y_lst.end());
        // auto min_it = std::min_element(y_lst.begin(), y_lst.end());
        // std::cout << "Max value = " << *max_it << "\tMin value = " << *min_it << std::endl;
    }

    void Hartree_Numerov(const std::vector<double>& r_grid, const std::vector<double>& density, std::vector<double>& U_Hartree, int Ntot){
        std::vector<double> h_Hartree(r_grid.size());
        std::vector<double> Y_Hartree(r_grid.size());
        for(std::size_t i = 0; i < r_grid.size(); ++i){
            h_Hartree[i] = -4. * PI * std::pow(r_grid[i], 5./2.) * density[i];
        }
        double Y2BC_init;
        auto Solve_Y = [&](double Y2BC_init, std::vector<double>& Y_Hartree){
            Y_Hartree.back() = static_cast<double>(Ntot) / std::sqrt(rmax);
            *(Y_Hartree.end()-2) = Y2BC_init;
            for (std::size_t i = (Y_Hartree.size() - 2); i > 0; --i)
            {
                Y_Hartree[i-1] = (2. * Y_Hartree[i] * (1. + (5./48.) * log_step * log_step) - Y_Hartree[i+1] * (1. - (1./48.) * log_step * log_step) 
                                + (h_Hartree[i+1] + h_Hartree[i-1] + 10. * h_Hartree[i]) * log_step * log_step / 12.) / (1. - (1./48.) * log_step * log_step);
            }
        };
        auto Solve_Y_Newton = [&](double Y2BC, std::vector<double>& Y_Hartree){
            int iter = 0;
            double alpha = 1E-1 / std::sqrt(rmax); //@ 1E-3
            const double tol = 1E-9 / std::sqrt(rmin); //@ 1E-4; 1E-9
            double residue_prev = 0;
            const int iter_max = 1000;
            std::vector<double> Y_Hartree_new(r_grid.size(), 0.);
            while (iter < iter_max)
            {
                Solve_Y(Y2BC, Y_Hartree);
                double residue = Y_Hartree.front();
                if (std::abs(residue) < tol)
                {
                    break;
                }
                // To get dU_da
                double Y2BC_new = Y2BC + alpha;
                Solve_Y(Y2BC_new, Y_Hartree_new);
                double dY_da = (Y_Hartree_new.front() - Y_Hartree.front()) / alpha;
                Y2BC -= residue / dY_da;
                if (residue_prev * residue < 0)
                {
                    alpha *= 0.5;
                }
                residue_prev = residue;
                ++iter;
            }
            if(iter >= iter_max){
                std::cout << "Error: Hartree reached Max Iteration! Hartree Config:" << std::endl;
                std::cout << "iter = " << iter << "\tU_Hartree[0] = " << U_Hartree.front() <<"\tSecond boundary condition. U_Hartree[Nx-2] =" << Y2BC <<"\tSteps =" << alpha << std::endl;
            }
            else{
                std::cout << "Done: Hartree converged via Progressive Refinement! Hartree Config:" << std::endl;
                std::cout << "iter = " << iter << "\tU_Hartree[0] = " << U_Hartree.front() <<"\tSecond boundary condition. U_Hartree[Nx-2] =" << Y2BC <<"\tSteps =" << alpha << std::endl;
            }
        };
        auto Y_2_U = [&](const std::vector<double>& Y_Hartree, std::vector<double>& U_Hartree)
        {
            for (std::size_t i = 0; i < Y_Hartree.size(); ++i)
            {
                U_Hartree[i] = Y_Hartree[i] * std::sqrt(r_grid[i]);
            }
        };
        Y2BC_init = ((static_cast<double>(Ntot) - 1E-8)/ std::sqrt(rmax));//@ 1E-8
        Solve_Y_Newton(Y2BC_init, Y_Hartree);
        Y_2_U(Y_Hartree, U_Hartree);
        Write_xy(r_grid, U_Hartree, "U_Hartree");
    }
    //Compute K-S potential
    class KS_Potential{ //Ctors: r_grid; U_Hartree; density
    private:
        std::vector<double> r_effective;
        const double Z_nucleus;
    public:
        std::vector<double>& V_exchange;
        std::vector<double>& E_exchange;
        std::vector<double>& V_correlation;
        std::vector<double>& E_correlation;
        std::vector<double>& V_effective;
        const std::vector<double>& r_grid;
        const std::vector<double>& U_Hartree;
        const std::vector<double>& density;
        KS_Potential(const std::vector<double>& r_grid_ctors, const std::vector<double>& U_Hartree_ctors, const std::vector<double>& density_ctors,
            std::vector<double>& V_exchange_ctors, std::vector<double>& E_exchange_ctors, 
            std::vector<double>& V_correlation_ctors, std::vector<double>& E_correlation_ctors, std::vector<double>& V_effective_ctors, double Z_nucleus_ctors)
            :r_grid(r_grid_ctors), U_Hartree(U_Hartree_ctors), density(density_ctors),
            V_exchange(V_exchange_ctors), E_exchange(E_exchange_ctors), 
            V_correlation(V_correlation_ctors), E_correlation(E_correlation_ctors), V_effective(V_effective_ctors),
            Z_nucleus(Z_nucleus_ctors){
                r_effective.resize(r_grid.size());
            }
        void Initialize_rs(){
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                if(density[i] > density_smear_cutoff){
                    r_effective[i] = std::pow(3. / (4. * PI * density[i]), 1./3.);
                }
                else{
                    r_effective[i] = std::pow(3. / (4. * PI * density_smear_cutoff), 1./3.);
                }
            }
        }
        void Exchange(){
            double x_coef = -1. * std::pow(3./(2. * PI), 2./3.);
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                if(density[i] > density_smear_cutoff){
                    V_exchange[i] = x_coef / r_effective[i];
                    E_exchange[i] = .75 * x_coef / r_effective[i];
                }
                else{
                    V_exchange[i] = 0.;
                    E_exchange[i] = 0.;
                }
            }
        }
        void Correlation(){
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                double x = std::sqrt(r_effective[i]);
                double X_func = x * x + b * x + c;
                double term1 = .5 * A * (std::log(x * x / X_func) + std::atan(Q / (2. * x + b)) * 2. * b / Q);
                double term2 = (-0.5 * A * b * x0 / (x0 * x0 + b * x0 + c)) * (std::log((x - x0) * (x - x0) / X_func) + std::atan(Q / (2. * x + b)) * 2. * (b + 2. * x0) / Q);
                double term3 = (-1./6.) * A * (c * (x - x0) - b * x * x0) / ((x - x0) * X_func);
                if(density[i] > density_smear_cutoff){
                    V_correlation[i] = term1 + term2 + term3;
                    E_correlation[i] = term1 + term2;
                }
                else{
                    V_correlation[i] = 0.;
                    E_correlation[i] = 0.;
                }
            }
        }
        void Wrap_effective(){
            Initialize_rs();
            Exchange();
            Correlation();
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                V_effective[i] = (-1. * Z_nucleus / r_grid[i]) + (U_Hartree[i] / r_grid[i]) + V_exchange[i] + V_correlation[i];
            }
        }
    };

    int Solve_Schrodinger(const std::vector<double>& r_grid, const std::vector<double>& V_effective, OrbitalStruct& orbital, double &E_start){
        int n = orbital.Orb_n;
        int l = orbital.Orb_l;
        const int Total_Nodes = n - l - 1;
        int nodes_prev = 1000;
        const int iter_max = 5000; //@v9 3000
        int iter = 0;
        double tol_dE = 1E-18;
        double tol = 1E-7  / std::sqrt(rmin); //@ 1E-6; less than E_converge is required!
        double dE = 1E-1; //@ 1E-1 or 1E-2
        double Enl;
        double residue_prev = 0.;
        auto Solve_ynl = [&](double energy, std::vector<double>& ynl){ //lambda: [capture list that compose func body. use `&` to save memory from copy `=`] (parameter list that lambda works on) { body }
            std::vector<double> p_KS(r_grid.size());
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                p_KS[i] = 2. * (V_effective[i] - energy )* r_grid[i] * r_grid[i] + static_cast<double>(l * (l+1)) + 1./4.;
            }
            ynl.back() = 0.;
            *(ynl.end() - 2) = 1E-6 / std::sqrt(rmax); //@ 1E-6
            for (std::size_t i = (r_grid.size() - 2); i >0 ; --i){
                ynl[i-1] = (2.*ynl[i]*(1.+ (5./12.)*p_KS[i]*log_step*log_step) - ynl[i+1]*(1.- (1./12.)*p_KS[i+1]*log_step*log_step))/
                        (1.-(1./12.)*p_KS[i-1]*log_step*log_step);
            }
        };
        auto Count_nodes = [&](const std::vector<double>& ynl){
            int nodes = 0;
            for (std::size_t i = 6; i < ynl.size() - 5; ++i) { // skip the boundary region
                if (ynl[i] * ynl[i - 1] < 0) {
                    nodes++;
                }
            }
            return nodes;
        };
        auto y_2_unl = [&](const std::vector<double>& ynl, std::vector<double>& unl)
        {
            for (std::size_t i = 0; i < ynl.size(); ++i)
            {
                unl[i] = ynl[i] * std::sqrt(r_grid[i]);
            }
        };
        auto Normalize_unl = [&](std::vector<double> &unl)
        {
            double norm_factor = 0.;
            double f_iminus1, f_i, f_iplus1;
            for (std::size_t i = 1; i < r_grid.size(); i += 2)
            {
                if (i == 1)
                {
                    f_iminus1 = unl[i - 1] * unl[i - 1];
                }
                else
                {
                    f_iminus1 = f_iplus1;
                }
                f_i = unl[i] * unl[i];
                f_iplus1 = unl[i + 1] * unl[i + 1];
                norm_factor += (f_iminus1 + 4. * f_i + f_iplus1) * (r_grid[i + 1] - r_grid[i - 1]) / 6.;
            }
            norm_factor = std::sqrt(norm_factor);
            for (std::size_t i = 0; i < r_grid.size(); ++i)
            {
                unl[i] /= norm_factor;
            }
        };
        std::vector<double> ynl(r_grid.size(), 0.);
        std::vector<double> unl(r_grid.size(), 0.);
        Enl = E_start;
        while (iter < iter_max && Enl < 0.0) {
            Solve_ynl(Enl, ynl);
            int nodes = Count_nodes(ynl);
            if((std::abs(ynl.front()) < tol || dE < tol_dE) && nodes == Total_Nodes) {
                break;
            }
            if (residue_prev * ynl.front() < 0.0 && nodes_prev == Total_Nodes) {
                dE *= 0.5;
                Enl -= dE;
                // std::cout << "iter = " << iter << "\tE = " << Enl << "\tdE = " << dE << "\tunl boundary = " << unl.front() << "\tn = " << n << "\tl = " << l << "\tPrev nodes = " << nodes_prev << "\tTarget nodes = " << Total_Nodes << std::endl;
            } else {
                residue_prev = ynl.front();
                nodes_prev = nodes;
                Enl += dE;
            }
            ++iter;
        }
        y_2_unl(ynl, unl);
        // Normalize WF
        Normalize_unl(unl);
        orbital.Orb_Enl = Enl;
        orbital.Orb_unl = unl;
        if (std::abs(ynl.back()) < tol && nodes_prev == Total_Nodes)
        {
            std::cout << "[✔] Done: Schrodinger converged via Bisection Numerov! Wavefunction Config:" << std::endl;
            std::cout << "iter = " << iter << "\tE = " << Enl << "\tdE = " << dE << "\tunl boundary = " << unl.front() << "\tn = " << n << "\tl = " << l << "\tCurrent nodes = " << nodes_prev << "\tTarget nodes = " << Total_Nodes << std::endl;
            return 0;
        }
        else //if (iter >= iter_max || Enl >= 0.)
        {
            std::cout << "[✘] Error: Schrodinger did not converge! Wavefunction Config:" << std::endl;
            std::cout << "iter = " << iter << "\tE = " << Enl << "\tdE = " << dE << "\tunl boundary = " <<  unl.front() << "\tn = " << n << "\tl = " << l << "\tCurrent nodes = " << nodes_prev << "\tTarget nodes = " << Total_Nodes << std::endl;
            return 1;
        }
    }

    std::tuple<double, double, double> Wrap_TotalEnergy(const std::vector<double> &r_grid, const std::vector<double> &density, const std::vector<double> &U_Hartree,
                                                        const std::vector<double> &V_exchange, const std::vector<double> &E_exchange, 
                                                        const std::vector<double> &V_correlation, const std::vector<double> &E_correlation, const std::vector<OrbitalStruct> &Atom)
    {
        double Etot = 0.;
        double E_Hartree_integrate = 0.;
        double E_ExC_integrate = 0.;
        double E_leftovers = 0.;
        double density_integrate = 0.;
        double f_iminus1, f_i, f_iplus1;
        for (const OrbitalStruct &x : Atom)
        {
            Etot += x.Orb_Nnl * x.Orb_Enl;
        }
        for (std::size_t i = 1; i < r_grid.size(); i += 2)
        {
            if (i == 1)
            {
                f_iminus1 = PI * r_grid[i - 1] * density[i - 1] * (2. * U_Hartree[i - 1]);
            }
            else
            {
                f_iminus1 = f_iplus1;
            }
            f_i = PI * r_grid[i] * density[i] * (2. * U_Hartree[i]);
            f_iplus1 = PI * r_grid[i + 1] * density[i + 1] * (2. * U_Hartree[i + 1]);
            E_Hartree_integrate += (f_iminus1 + 4. * f_i + f_iplus1) * (r_grid[i + 1] - r_grid[i - 1]) / 6.;
        }
        for (std::size_t i = 1; i < r_grid.size(); i += 2)
        {
            if (i == 1)
            {
                f_iminus1 = -4. * PI * r_grid[i - 1] * r_grid[i - 1] * density[i - 1] * (V_exchange[i - 1] + V_correlation[i - 1]);
            }
            else
            {
                f_iminus1 = f_iplus1;
            }
            f_i = -4. * PI * r_grid[i] * r_grid[i] * density[i] * (V_exchange[i] + V_correlation[i]);
            f_iplus1 = -4. * PI * r_grid[i + 1] * r_grid[i + 1] * density[i + 1] * (V_exchange[i + 1] + V_correlation[i + 1]);
            E_leftovers += (f_iminus1 + 4. * f_i + f_iplus1) * (r_grid[i + 1] - r_grid[i - 1]) / 6.;
        }
        for (std::size_t i = 1; i < r_grid.size(); i += 2)
        {
            if (i == 1)
            {
                f_iminus1 = 4. * PI * r_grid[i - 1] * r_grid[i - 1] * density[i - 1] * (E_exchange[i - 1] + E_correlation[i - 1]);
            }
            else
            {
                f_iminus1 = f_iplus1;
            }
            f_i = 4. * PI * r_grid[i] * r_grid[i] * density[i] * (E_exchange[i] + E_correlation[i]);
            f_iplus1 = 4. * PI * r_grid[i + 1] * r_grid[i + 1] * density[i + 1] * (E_exchange[i + 1] + E_correlation[i + 1]);
            E_ExC_integrate += (f_iminus1 + 4. * f_i + f_iplus1) * (r_grid[i + 1] - r_grid[i - 1]) / 6.;
        }
        Etot += E_ExC_integrate + E_leftovers - E_Hartree_integrate;
        return std::make_tuple(E_Hartree_integrate, E_ExC_integrate, Etot);
    }

    void Update_density(const std::vector<double>& r_grid, const std::vector<OrbitalStruct>& Atom, std::vector<double>& density){
        std::vector<double> new_density(r_grid.size());
        for(const OrbitalStruct& x : Atom){
            for(std::size_t i = 0; i < r_grid.size(); ++i){
                new_density[i] += x.Orb_Nnl * x.Orb_unl[i] * x.Orb_unl[i] / (4. * PI * r_grid[i] * r_grid[i]);
            }
        }
        density = new_density;
    }

    int main(){
        auto [Atom_config, atom_name] = Select_Atom();
        const int Ntot = Atom_config.Ntot;
        std::vector<OrbitalStruct> Atom = Atom_config.orbitals;
        const double Z_nucleus = static_cast<double>(Atom_config.Ntot);
        InitializeStruct init = Initialize_n(Z_nucleus, Ntot);
        std::vector<double> V_exchange(init.grid.size());
        std::vector<double> E_exchange(init.grid.size());
        std::vector<double> V_correlation(init.grid.size());
        std::vector<double> E_correlation(init.grid.size());
        std::vector<double> V_effective(init.grid.size());
        std::vector<double> r_grid = init.grid;
        std::vector<double> U_Hartree(init.grid.size());
        std::vector<double> density = init.density;
        std::vector<double> density_prev(density.size());
        std::vector<double> TotalEnergy_history{};
        std::cout << std::scientific << std::setprecision(5);
        int iter = 0;
        const int Iter_max_test = 200; // remove later
        int error_code;
        bool check_converge;
        double E_Hartree_integrate, E_ExC_integrate, Etot, EDiff;
        while(iter < Iter_max_test){
            check_converge = true;
            std::cout << "------Starting Main Loop Iteration = " << iter << std::endl;
            Hartree_Numerov(r_grid, density, U_Hartree, Ntot);//step2: Update U_Hartree  //correct on log grid!
            KS_Potential step3(r_grid, U_Hartree, density,
                            V_exchange, E_exchange, V_correlation, E_correlation, V_effective, Z_nucleus); //step3: Update this line
            step3.Wrap_effective();
            double E_start = -150.; //@v9 -50 //@v10 -100 < P @v10 -150 < Ca
            for(OrbitalStruct& orbital: Atom){//Step4: Update Atom
                error_code = Solve_Schrodinger(r_grid, V_effective, orbital, E_start);
                if(error_code != 0){
                    check_converge = false;
                }
            }
            density_prev.swap(density);
            Update_density(r_grid, Atom, density);
            for (std::size_t i = 0; i < density.size(); ++i) {
                density[i] = 0.5 * density[i] + 0.5 * density_prev[i];
            }
            std::tie(E_Hartree_integrate, E_ExC_integrate, Etot) = Wrap_TotalEnergy(r_grid, density, U_Hartree, V_exchange, E_exchange, V_correlation, E_correlation, Atom);//step5: Wrap up total Energy
            TotalEnergy_history.push_back(Etot);
            printf("Etot = %f", Etot);
            // Write_xy(r_grid, density, "10test_n");
            // return 0; 
            ++iter;
            std::cout << "------Done: Main Loop Iteration = " << iter << "\tTotal Energy = " << Etot << std::endl;
            if(TotalEnergy_history.size() >= 2) {
                EDiff = TotalEnergy_history.back() - *(TotalEnergy_history.end() - 2); 
                std::cout << "Energy difference: Total Energy [" << iter - 1 << "] = " << TotalEnergy_history[iter - 1]
                        << "\tTotal Energy [" << (iter - 2) << "] = "  << TotalEnergy_history[iter - 2]
                        << "\tEDiff =" << EDiff << std::endl;
                if(std::abs(EDiff) < E_converge && check_converge){
                    std::cout << "Converged! Writing wavefunction unl ..." << std::endl;
                    for(const OrbitalStruct& x : Atom){
                        int Total_Nodes = x.Orb_n - x.Orb_l - 1;
                        Write_wavefunction(x.Orb_n, x.Orb_l, Total_Nodes, x.Orb_unl, r_grid, atom_name);
                    }
                    std::cout << "All job done! Final atomic config:" << std::endl;
                    for(const auto& orb : Atom) {
                        std::cout << "\tn = " << orb.Orb_n << "\tl = " << orb.Orb_l
                                << "\tOccupancy = " << orb.Orb_Nnl << "\tEnl = " << orb.Orb_Enl << std::endl;
                    }
                    std::cout << "Energy info:" << std::endl;
                    std::cout << "\tE_Hartree = " << E_Hartree_integrate << "\t\tE_Exchange_Correlation = " << E_ExC_integrate
                                << "\t\tTotal Energy = " << Etot << std::endl;
                    break;
                }
            }
        }
        // for(const OrbitalStruct& x : Atom){
        //     Write_xy(r_grid, x.Orb_unl, "10test_unl_final");
        // }
        return 0;
    }
    // Pain in the ass... :)
            