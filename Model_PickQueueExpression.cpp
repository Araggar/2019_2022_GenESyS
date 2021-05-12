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

#include "Model_PickQueueExpression.h"

// you have to included need libs

// GEnSyS Simulator
#include "Simulator.h"

// Model Components
#include "Create.h"
#include "Delay.h"
#include "Dispose.h"
#include "PickQueue.h"

// Model model
#include "EntityType.h"

Model_PickQueueExpression::Model_PickQueueExpression() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Model_PickQueueExpression::main(int argc, char** argv) {
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
	PickQueue* pq = new PickQueue(model);
	Queue* queue1 = new Queue(model, "Q1");
	Queue* queue2 = new Queue(model, "Q1");
	Queue* queue3 = new Queue(model, "Q1");
	Queue* queue4 = new Queue(model, "Q1");

	pq->addQueue(queue1);
	pq->addQueue(queue2);
	pq->addQueue(queue3);
	pq->addQueue(queue4);

	Create* create1 = new Create(model);
	create1->setEntityType(entityType1);
	create1->setTimeBetweenCreationsExpression("1.5"); // create one new entity every 1.5 seconds

	//
	create1->getNextComponents()->insert(pq);

	//
	model->save("./models/Model_PickQueueExpression.txt");
	model->getSimulation()->start();
	genesys->~Simulator();
	return 0;
};

