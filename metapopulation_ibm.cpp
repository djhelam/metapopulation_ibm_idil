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

class TInd  //this defines a class of individuals
{
  public: 
  TInd();  
  double lambda;
  double alpha;
  double dispersal_probability;
};
TInd::TInd()
{
  lambda=0.0;
  alpha=0.0;
  dispersal_probability=0.0;
}

class TPatch
{
  public:
  TPatch();
  vector<TInd> females;
  vector<TInd> newfemales;
  double measured_dispersal;
};
TPatch::TPatch()
{
  females.clear();
  newfemales.clear();
  measured_dispersal=0.0;
}

const int NUMBER_OF_PATCHES=100;

TPatch world[NUMBER_OF_PATCHES];

int REPLICATES=1;
int N0=10;
double LAMBDA=2.0;
double ALPHA=0.01;
int TMAX=100;
double DISPERSAL_PROBABILITY=0.5;
double DISPERSAL_MORTALITY=0;
double MUTATION_PROBABILITY=0.0;
double MUTATION_EFFECT_SD=0.1;



/*multi line comment*/

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

void initialise_world()
{
  for(int x=0;x<NUMBER_OF_PATCHES;x++)
  {
    world[x].females.clear();
    world[x].newfemales.clear();
    world[x].measured_dispersal=0.0;
    for(int n=0;n<N0;n++)
    {
      TInd newind;
      newind.lambda=LAMBDA;
      newind.alpha=ALPHA;
      newind.dispersal_probability=DISPERSAL_PROBABILITY;
      world[x].females.push_back(newind);
    }
  }

}

int find_patch(int x)
{
  int new_patch;
  if(NUMBER_OF_PATCHES>1)
  {
    do
    {
      new_patch=floor(ran()*NUMBER_OF_PATCHES);
    }
    while(new_patch==x);
  }
  else new_patch=x;
  return new_patch;
}

void disperse()
{
  for(int x=0;x<NUMBER_OF_PATCHES;x++)
  {
    world[x].newfemales.clear();
    int population_size=world[x].females.size();
    int count_dispersers=0;
    for(int f=0; f<world[x].females.size();f++)
    {
      if(ran()<world[x].females.at(f).dispersal_probability)
      {
        count_dispersers++;
        int new_patch;
        new_patch=find_patch(x);
        if(ran()<1-DISPERSAL_MORTALITY)
        {
          world[new_patch].newfemales.push_back(world[x].females.at(f));
        }
        world[x].females.erase(world[x].females.begin()+f);
        f--;
      }
    }
    if(world[x].females.size()>0)
      world[x].measured_dispersal=double(count_dispersers)/double(population_size);
    else
      world[x].measured_dispersal=0.0;
    
  }
  for(int x=0;x<NUMBER_OF_PATCHES;x++)
  {
    for(int nf=0;nf<world[x].newfemales.size();nf++)
    {
      world[x].females.push_back(world[x].newfemales.at(nf));
    }
    world[x].newfemales.clear();
  }
}

double mutation(double dispersal)
{
  if(ran()<MUTATION_PROBABILITY)
  {
    dispersal=dispersal+gauss(MUTATION_EFFECT_SD);
  }
  return dispersal;
}

void reproduce()
{
  for(int x=0;x<NUMBER_OF_PATCHES;x++)
  {
    double alpha_sum=0.0;
    for(int f=0;f<world[x].females.size();f++)
    {
      alpha_sum=alpha_sum+world[x].females.at(f).alpha;
    }
    for(int f=0;f<world[x].females.size();f++)
    {
      double number_of_offspring=poisson(world[x].females.at(f).lambda/(1+alpha_sum));
      for(int no=0;no<number_of_offspring;no++)
      {
        TInd newind;
        newind.lambda=world[x].females.at(f).lambda;
        newind.alpha=world[x].females.at(f).alpha; 
        newind.dispersal_probability=mutation(world[x].females.at(f).dispersal_probability);
        world[x].newfemales.push_back(newind);   
      }
    }
  }
}

void death()
{
  for(int x=0;x<NUMBER_OF_PATCHES;x++)
  {
    world[x].females.clear();
    world[x].females=world[x].newfemales;
    world[x].newfemales.clear();
  }
}

//________________________________________________________________________________________
//---------------------------------------------------------------------------Main function
/*The program is always executed starting from the main function, even if we write other
functions before. Here, we will execute everything our IBM will do. This includes, the 
output of generated data, initialisation and running the life cycle of our modelled
organism.*/
int main()
{
  ofstream op;
  op.open("output.txt");
  op<<"r t x N measured_dispersal"<<endl;
  specify_rng(RS);
  for(int r=0;r<REPLICATES;r++)
  {
    initialise_world(); //here we initialise environement
    for(int t=0;t<TMAX;t++) //we go through the
    {
      for(int x=0;x<NUMBER_OF_PATCHES;x++)
      {
        op<<r<<" "<<t<<" "<<x<<" "<<world[x].females.size()<<" "<<world[x].measured_dispersal<<endl;
      }
     disperse();
     reproduce();
     death();
  }
  }
  op.close();


  return 0;
}







