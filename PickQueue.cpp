/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PickQueue.cpp
 * Author: rlcancian
 * 
 * Created on 11 de Setembro de 2019, 13:08
 */

#include "PickQueue.h"

#include "Model.h"
#include <assert.h>

PickQueue::PickQueue(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<PickQueue>(), name) {
}

std::string PickQueue::show() {
	return ModelComponent::show() + "";
}

ModelComponent* PickQueue::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	PickQueue* newComponent = new PickQueue(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void PickQueue::setMaximum() {
	this->minimum = false;
}

void PickQueue::setMinimum() {
	this->minimum = true;
}

void PickQueue::addStation(Station* station) {
//	this->_list->insert(station);
//	this->_listResource->insert(NULL);
//	this->_listQueue->insert(NULL);
}

void PickQueue::addResource(Resource* resource, Station* station) {
//	for (unsigned int it = 0; it < this->_list->size(); it++){
//		if (station == this->_list->getAtRank(it)) {
//			this->_listResource->setAtRank(it, resource);
//		}
//	}
}

void PickQueue::addQueue(Queue* queue) {
//	for (unsigned int it = 0; it < this->_list->size(); it++){
//		if (station == this->_list->getAtRank(it)) {
//			this->_listQueue->setAtRank(it, queue);
//		}
//	}
}

void PickQueue::setSelectionEnRoute() {
	this->selection = 1;
}

void PickQueue::setSelectionResource() {
	this->selection = 2;
}

void PickQueue::setSelectionQueue() {
	this->selection = 4;
}

void PickQueue::setTimeDelay(double delay) {
	this->_delay = delay;
}

Station* PickQueue::pickEnRoute() {
	Station* chosen = this->_list->getAtRank(0);
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		if (chosen->getNumberInStation() > this->_list->getAtRank(it)->getNumberInStation()) {
			chosen = this->_list->getAtRank(it);
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "EnRoute \"" + std::to_string(chosen->getNumberInStation()) + "\"");
	//this->_listEnRoute->setAtRank(chosen, this->_listEnRoute->getAtRank(chosen) + 1);
	return chosen;
}

Station* PickQueue::pickResource() {
	assert (this->_list->size() == this->_listResource->size());
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		if (this->_listResource->getAtRank(chosen)->getNumberBusy() > this->_listResource->getAtRank(it)->getNumberBusy()) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Resource \"" + std::to_string(this->_listResource->getAtRank(chosen)->getNumberBusy()) + "\"");
	return this->_list->getAtRank(chosen);
}

Station* PickQueue::pickQueue() {
	assert (this->_list->size() == this->_listQueue->size());
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		if (this->_listQueue->getAtRank(chosen)->size() > this->_listQueue->getAtRank(it)->size()) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(this->_listQueue->getAtRank(chosen)->size()) + "\"");
	return this->_list->getAtRank(chosen);
}

void PickQueue::_execute(Entity* entity) {
	//assert(!_list->empty());
	//Station* chosen;

//	switch (this->selection) {
//		case 1 :
//			chosen = pickEnRoute();
//			break;
//		case 2 :
//			chosen = pickResource();
//			break;
//		case 4 :
//			chosen = pickQueue();
//			break;
//	}

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected na");
	//this->_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
	//this->_parentModel->sendEntityToComponent(entity, chosen->getEnterIntoStationComponent(), this->_delay);
	//_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
}

bool PickQueue::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		//...
	}
	return res;
}

void PickQueue::_initBetweenReplications() {
}

std::map<std::string, std::string>* PickQueue::_saveInstance() {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
	//...
	return fields;
}

bool PickQueue::_check(std::string* errorMessage) {
	bool resultAll = true;
	//...
	return resultAll;
}

PluginInformation* PickQueue::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<PickQueue>(), &PickQueue::LoadInstance);
	// ...
	info->setSendTransfer(true);
	return info;
}

