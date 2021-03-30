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
	file.open("Gumbell(3,4)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleGumbell(3,4) << std::endl;
	}
	file.close();
	
	sampler.reset();
	std::extreme_value_distribution<> ed(3,4);
	file.open("StdGumbell(3,4)-Output");
	for (int i = 0; i < 1000; i++) {
		file << ed(gen) << std::endl;
	}
	file.close();
	
	// Bernoulli 
	sampler.reset();
	file.open("Bernoulli(0.2)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleBernoulli(0.2) << std::endl;
	}
	file.close();
	
	sampler.reset();
	std::bernoulli_distribution berd(0.2);
	file.open("StdBernoulli(0.2)-Output");
	for (int i = 0; i < 1000; i++) {
		file << berd(gen) << std::endl;
	}
	file.close();
	
	// Discrete
	sampler.reset();
	file.open("Discrete(10,[4,1,2,3],[1,2,3,4],4)-Output");
	double values[4] = {1,2,3,4};
	double prob[4] = {4,2,1,3};
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleDiscrete(10,prob,values,4) << std::endl;
	}
	file.close();
	
	// Cauchy
	sampler.reset();
	file.open("Cauchy(0,1)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleCauchy(0,1) << std::endl;
	}
	file.close();
	
//	sampler.reset();
//	std::bernoulli_distribution berd(0.2);
//	file.open("StdBernoulli(0.2)-Output");
//	for (int i = 0; i < 1000; i++) {
//		file << berd(gen) << std::endl;
//	}
//	file.close();

	sampler.reset();
	file.open("Beta(0.5,0.5)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleBeta2(0.5,0.5) << std::endl;
	}
	file.close();

	//sampler.reset();

	file.open("Gamma(2.5,2)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleGamma2(2.5,2) << std::endl;
	}
	file.close();
	
	file.open("Gamma(0.5,1)-Output");
	for (int i = 0; i < 1000; i++) {
		file << sampler.sampleGamma2(0.5,1) << std::endl;
	}
	file.close();

	// Binomial & std::binomial_distribution
	sampler.reset();
	file.open("sampleBinomial(15,0.5)-Output");
		for(int i = 0; i < 1000; i++){
		file << sampler.sampleBinomial(15,0.5) << std::endl;
	}
	file.close();
	
	sampler.reset();
	std::binomial_distribution<> bd(15,0.5);
	file.open("sampleSTDBinomial(15,0.5)-Output");
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

