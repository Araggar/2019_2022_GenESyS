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

#include "Model_PickStationQueue.h"

// you have to included need libs

// GEnSyS Simulator
#include "Simulator.h"

// Model Components
#include "Create.h"
#include "Delay.h"
#include "Dispose.h"
#include "Queue.h"
#include "Enter.h"
#include "Dispose.h"
#include "Station.h"
#include "PickStation.h"

// Model model
#include "EntityType.h"

Model_PickStationQueue::Model_PickStationQueue() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Model_PickStationQueue::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	// insert "fake plugins" since plugins based on dynamic loaded library are not implemented yet
	this->insertFakePluginsByHand(genesys);
	// Handle traces and simulation events to output them
	this->setDefaultTraceHandlers(genesys->getTracer());
	genesys->getTracer()->setTraceLevel(Util::TraceLevel::L5_arrival);
	Model* model = genesys->getModels()->newModel();
	model->getSimulation()->setReplicationLength(60);
	EntityType* entityType1 = new EntityType(model, "Type_of_Representative_Entity");
	
	//
	PickStation* ps = new PickStation(model);
	
	Station* station1 = new Station(model, "S1");
	Station* station2 = new Station(model, "S2");
	Station* station3 = new Station(model, "S3");
	Station* station4 = new Station(model, "S4");

	Resource* resource1 = new Resource(model, "R1");
	Resource* resource2 = new Resource(model, "R2");
	Resource* resource3 = new Resource(model, "R3");
	Resource* resource4 = new Resource(model, "R4");
	
	ps->addStation(station1, resource1);
	ps->addStation(station2, resource2);
	ps->addStation(station3, resource3);
	ps->addStation(station4, resource4);

	Create* create1 = new Create(model);
	create1->setEntityType(entityType1);
	create1->setTimeBetweenCreationsExpression("1.5"); // create one new entity every 1.5 seconds

	Enter* enter1 = new Enter(model);
	enter1->setStation(station1);	
	enter1->setDescription("Enter1");

	Enter* enter2 = new Enter(model);
	enter2->setStation(station2);	
	enter2->setDescription("Enter2");
	
	Enter* enter3 = new Enter(model);
	enter3->setStation(station3);	
	enter3->setDescription("Enter3");

	Enter* enter4 = new Enter(model);
	enter4->setStation(station4);	
	enter4->setDescription("Enter4");

	Dispose* dispose1 = new Dispose(model);
	Dispose* dispose2 = new Dispose(model);

	//
	create1->getNextComponents()->insert(ps);
	enter1->getNextComponents()->insert(dispose1);
	enter2->getNextComponents()->insert(dispose1);

	enter3->getNextComponents()->insert(dispose2);
	enter4->getNextComponents()->insert(dispose2);

	//
	model->save("./models/Model_PickStationQueue.txt");
	model->getSimulation()->start();
	genesys->~Simulator();
	return 0;
};

