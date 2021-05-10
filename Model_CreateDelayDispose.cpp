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

#include "Model_CreateDelayDispose.h"

// you have to included need libs

// GEnSyS Simulator
#include "Simulator.h"

// Model Components
#include "Create.h"
#include "Delay.h"
#include "Dispose.h"
#include "Station.h"
#include "Route.h"
#include "Enter.h"
#include "PickStation.h"
#include "Queue.h"
#include "PickQueue.h"

// Model model
#include "EntityType.h"

Model_CreateDelayDispose::Model_CreateDelayDispose() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Model_CreateDelayDispose::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	// insert "fake plugins" since plugins based on dynamic loaded library are not implemented yet
	this->insertFakePluginsByHand(genesys);
	// Handle traces and simulation events to output them
	this->setDefaultTraceHandlers(genesys->getTracer());
	genesys->getTracer()->setTraceLevel(Util::TraceLevel::L5_arrival);
	Model* model = genesys->getModels()->newModel();
	//model->load("./models/Model_CreateDelayDispose.txt");
	//model->getSimulation()->start();
	//return 0;
	//
	// build the simulation model
	// if no ModelInfo is provided, then the model will be simulated once (one replication) and the replication length will be 3600 seconds (simulated time)
	model->getSimulation()->setReplicationLength(30);
	// create a (Source)ModelElement of type EntityType, used by a ModelComponent that follows
	EntityType* entityType1 = new EntityType(model, "Type_of_Representative_Entity");
	// create a ModelComponent of type Create, used to insert entities into the model
	Create* create1 = new Create(model);

	
	//Station* station1 = new Station(model, "Station 1");
	//Station* station2 = new Station(model, "Station 2");
	//
	//PickQueue* ps = new PickQueue(model);
	//ps->addStation(station1);
	//ps->addStation(station2);
	
	//Route* route1 = new Route(model);	
	//route1->setStation(station1);
	Queue* queue1 = new Queue(model, "Queue 1");
	Queue* queue2 = new Queue(model, "Queue 2");
	Queue* queue3 = new Queue(model, "Queue 3");
	Queue* queue4 = new Queue(model, "Queue 4");
	PickQueue* pq = new PickQueue(model, "PickQueue");
	pq->addQueue(queue1);
	pq->addQueue(queue2);
	pq->addQueue(queue3);
	pq->addQueue(queue4);

	//pq->setCyc();
	pq->setRandom();



	//Enter* enter1 = new Enter(model);
	//enter1->setStation(station1);	
	//enter1->setDescription("Enter1 Desc");

	//Enter* enter2 = new Enter(model);
	//enter2->setStation(station2);	
	//enter2->setDescription("Enter2 Desc");
	
	create1->setEntityType(entityType1);
	create1->setTimeBetweenCreationsExpression("0.5"); // create one new entity every 1.5 seconds
	// create a ModelComponent of type Delay, used to represent a time delay
	Delay* delay1 = new Delay(model);
	// if nothing else is set, the delay will take 1 second
	// create a (Sink)ModelComponent of type Dispose, used to remove entities from the model
	//PickStation* ps = new PickStation(model);
	Dispose* dispose1 = new Dispose(model); // insert the component into the model
	// connect model components to create a "workflow" -- should always start from a SourceModelComponent and end at a SinkModelComponent (it will be checked)
	
	//enter1->getNextComponents()->insert(delay1);

	//enter2->getNextComponents()->insert(delay1);

	create1->getNextComponents()->insert(pq);
	pq->getNextComponents()->insert(delay1);
	//pq->getNextComponents()->insert(delay1);
	delay1->getNextComponents()->insert(dispose1);
	//enter1->getNextComponents()->insert(dispose1);
	//enter1->getNextComponents()->insert(ps);
	//ps->getNextComponents()->insert(dispose1);
	// save the model into a text file
	


	model->save("./models/Model_CreateDelayDispose.txt");
	// execute the simulation util completed and show the report
	model->getSimulation()->start();
	genesys->~Simulator();
	return 0;
};

