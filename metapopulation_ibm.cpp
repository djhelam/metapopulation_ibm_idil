/*
  Copyright (C) 2023  Jhelam N. Deshpande
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
*/

//_______________________________________________________________________________________
//-----------------------------------------------------We include all required libraries
//Libraries contain functions that we may need
#include <cstdlib>
#include <iostream> //standard input/output functions like cout/cin
#include <fstream>  //functions required to read and write to file
#include <string>   
#include <vector>   //vector functions
#include <gsl/gsl_rng.h>  //functions to generate random numbers     
#include <gsl/gsl_randist.h>  //functions to generate random numbers
#include <math.h>
#include <algorithm>
using namespace std; //this generally not good coding practice

//________________________________________________________________________________________
//------------------------------------------------------Initialize Random Number Generator
const gsl_rng *gBaseRand;

void specify_rng(unsigned long randSeed)
{
  gBaseRand = gsl_rng_alloc(gsl_rng_rand);

  srand(randSeed);
  unsigned long r = rand();
  gsl_rng_set(gBaseRand, r);
}

//________________________________________________________________________________________
//-------------------------------------------------------------------------Simplifications

//-------------------------------------------------Simplify Random Drawing between 0 and 1

double ran()
{
  return gsl_rng_uniform(gBaseRand);
}

//---------------------------------------------------------------Simplify Gaussian Randoms

double gauss(double sd)
{
  return gsl_ran_gaussian(gBaseRand,sd);
}

//-----------------------------------------------------------------Simplify Poisson Random

int poisson(double sd)
{
  return gsl_ran_poisson(gBaseRand,sd);
}


const int RS = 100;                 // random seed

//________________________________________________________________________________________
//---------------------------------------------------------------------------Main function
/*The program is always executed starting from the main function, even if we write other
functions before. Here, we will execute everything our IBM will do. This includes, the 
output of generated data, initialisation and running the life cycle of our modelled
organism.*/
int main()
{
  return 0;
}







