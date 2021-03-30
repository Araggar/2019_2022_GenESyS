/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstExampleOfSimulation.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include <random>
#include <fstream>
#include <iostream>

#include "FirstExampleOfSimulation.h"

#include "SamplerDefaultImpl1.h"

FirstExampleOfSimulation::FirstExampleOfSimulation() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int FirstExampleOfSimulation::main(int argc, char** argv) {

	std::random_device rd;
	std::mt19937 gen(rd());

	SamplerDefaultImpl1 sampler = SamplerDefaultImpl1();

	std::ofstream file;
//	file.open("Chi2(2)-Output");
//
//	for (int i = 0; i < 1000; i++) {
//		file << sampler.sampleChiSqrt(2) << std::endl;
//	}
//	file.close();

	// Gumbell 
	sampler.reset();
	file.open("Gumbell(1,2)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleGumbell(1,2) << std::endl;
	}
	file.close();

	//sampler.reset();
	//file.open("Beta(2,5)-Output");
	//for (int i = 0; i < 1000; i++) {
	//	file << sampler.sampleBetaPDF(2,5) << std::endl;
	//}
	//file.close();

	//sampler.reset();

	//file.open("Gamma(2,5)-Output");
	//for (int i = 0; i < 1000; i++) {
	//	file << sampler.sampleGammaPDF(2,5) << std::endl;
	//}
	//file.close();

	// Binomial & std::binomial_distribution
	sampler.reset();
	file.open("sampleBinomial(15)-Output");
		for(int i = 0; i < 1000; i++){
		file << sampler.sampleBinomial(15) << std::endl;
	}
	file.close();
	
	sampler.reset();
	std::binomial_distribution<> bd(15);
	file.open("sampleSTDBinomial(15)-Output");
		for(int i = 0; i < 1000; i++){
		file << bd(gen) << std::endl;
	}
	file.close();

	// Geometric and std::geometric_distribution
	sampler.reset();
	file.open("sampleGeometric(0.5)-Output");
		for(int i = 0; i < 1000; i++){
		file << sampler.sampleGeometric(0.5) << std::endl;
	}
	file.close();
	
	sampler.reset();
	std::geometric_distribution<> gd(0.5);	
	file.open("sampleSTDGeometric(0.5)-Output");
		for(int i = 0; i < 1000; i++){
		file << gd(gen) << std::endl;
	}
	file.close();

	return 0;
};

