//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
///  \file
///
///  declaration for scanner module
///
///  *********************************************
///
///  Authors (add name and date if you modify):
//
///  \author Gregory Martinez
///          (gregory.david.martinez@gmail.com)
///  \date Feb 2014
///
///  *********************************************

#ifndef CHOLESKY_HPP
#define CHOLESKY_HPP

#include <cmath>

namespace Gambit
{
        // Cholesky decomposition class
        class Cholesky
        {
        private:
                std::vector<std::vector<double>> el;
                
        public:
                Cholesky(const int num) : el(num, std::vector<double>(num)) {}
                        
                bool EnterMat(std::vector<std::vector<double>> &a)
                {
                        int num = el.size();
                        double sum = 0;
                        int i, j, k;

                        el = a;
                        
                        for (i = 0; i < num; i++)
                        {
                                for (j = i; j < num; j++)
                                {
                                        for(sum = el[i][j], k = i - 1; k >= 0; k--)
                                                sum -= el[i][k]*el[j][k];
                                        if(i == j)
                                        {
                                                if(sum <= 0.0)
                                                {
                                                        return false;
                                                }
                                                el[i][i] = std::sqrt(sum);
                                        }
                                        else
                                                el[j][i] = sum/el[i][i];
                                }
                        }
                        
                        for (i = 0; i < num; i++)
                                for (j = 0; j < i; j++)
                                        el[j][i] = 0.0;
                                
                        return true;
                }
                
                void ElMult (std::vector<double> &y) const
                {
                        int i, j;
                        int num = el.size();
                        std::vector<double> b(num);
                        for(i = 0; i < num; i++)
                        {
                                b[i] = 0.0;
                                for (j = 0; j <= i; j++)
                                {
                                        b[i] += el[i][j]*y[j];
                                }
                        }
                        
                        y = b;
                }
        };
}

#endif