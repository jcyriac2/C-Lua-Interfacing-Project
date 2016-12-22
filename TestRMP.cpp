//Author: Alex Schwing
#include <iostream>

#include "Region.h"

//compile with: g++ TestRMP.cpp -std=c++11 -o prog
//execute: ./prog

//Output similar to
//Test...
//{some number}
//Beliefs:
//0 1
//0 1
//0 1


int main(int, char**) {
	std::cout << "Test..." << std::endl;

	double dataUnary[2]{0.0, 1.0};
	//double dataUnaryStrong[2]{1.0, 3.0};
	double dataPair[4]{1.0, 0.0, 0.0, 1.0};
	//double dataPairAsym[4]{3.5, 0.5, 0.2, 1.0};

	MPGraph<double, int> g;
	g.AddVariables({ 2, 2, 2 });
	std::vector<MPGraph<double, int>::PotentialID> pots;
	pots.push_back(g.AddPotential(MPGraph<double, int>::PotentialVector(dataUnary, 2)));
	//pots.push_back(g.AddPotential(MPGraph<double, int>::PotentialVector(dataUnary, 2)));
	//pots.push_back(g.AddPotential(MPGraph<double, int>::PotentialVector(dataUnary, 2)));
	pots.push_back(g.AddPotential(MPGraph<double, int>::PotentialVector(dataPair, 4)));
	//pots.push_back(g.AddPotential(MPGraph<double, int>::PotentialVector(dataPairAsym, 4)));

	std::vector<MPGraph<double, int>::RegionID> regs;
	for (int k = 0; k < 3; ++k) {
		regs.push_back(g.AddRegion(1.0, std::vector<int>{k}, pots[0]));
	}
	regs.push_back(g.AddRegion(1.0, { 0, 1 }, pots[1]));
	regs.push_back(g.AddRegion(1.0, { 1, 2 }, pots[1]));
	regs.push_back(g.AddRegion(1.0, { 0, 2 }, pots[1]));

	/*g.AddConnection(regs[0], regs[3]);
	g.AddConnection(regs[0], regs[5]);
	g.AddConnection(regs[1], regs[3]);
	g.AddConnection(regs[1], regs[4]);
	g.AddConnection(regs[2], regs[4]);
	g.AddConnection(regs[2], regs[5]);*/

	//g.AddConstraint({ { regs[0], 0 }, { regs[1], 1 }, { regs[2], 0 }, {regs[3], 3} });

	g.AllocateMessageMemory();

	double epsilon = 0.0;

	CPrecisionTimer CTmr;
	CTmr.Start();
	RMP<double, int> RMPAlgo;
	RMPAlgo.RunMP(g, epsilon, 10);
	std::cout << CTmr.Stop() << std::endl;

	double* beliefs;
	RMPAlgo.GetBeliefs(g, epsilon, &beliefs, true);

	std::cout << "Beliefs:" << std::endl;
	for(int k=0;k<3;++k) {
		for(int m=0;m<2;++m) {
			std::cout << beliefs[2*k+m] << " ";
		}
		std::cout << std::endl;
	}

	delete[] beliefs;

	return 0;
}
