/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PickStation.cpp
 * Author: rlcancian
 * 
 * Created on 11 de Setembro de 2019, 13:08
 */

#include "PickStation.h"

#include "Model.h"
#include <assert.h>

PickStation::PickStation(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<PickStation>(), name) {
}

std::string PickStation::show() {
	return ModelComponent::show() + "";
}

ModelComponent* PickStation::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	PickStation* newComponent = new PickStation(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void PickStation::setMaximum() {
	this->minimum = false;
}

void PickStation::setMinimum() {
	this->minimum = true;
}

void PickStation::addStation(Station* station) {
	this->_list->insert(station);
	this->_listEnRoute->insert(0);
}

void PickStation::setSelectionEnRoute() {
	this->selection = 1;
}

Station* PickStation::pickEnRoute() {
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		if (this->_listEnRoute->getAtRank(chosen) > this->_listEnRoute->getAtRank(it)) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "EnRoute \"" + std::to_string(this->_listEnRoute->getAtRank(chosen)) + "\"");
	this->_listEnRoute->setAtRank(chosen, this->_listEnRoute->getAtRank(chosen) + 1);
	return this->_list->getAtRank(chosen);
}

void PickStation::_execute(Entity* entity) {
	assert(!_list->empty());
	Station* chosen;

	switch (this->selection) {
		case 1 :
			chosen = pickEnRoute();
			break;
	}

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected " + chosen->getName());
	//this->_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
	this->_parentModel->sendEntityToComponent(entity, chosen->getEnterIntoStationComponent(), 0.0);
	//this->_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
}

bool PickStation::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		//...
	}
	return res;
}

void PickStation::_initBetweenReplications() {
}

std::map<std::string, std::string>* PickStation::_saveInstance() {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
	//...
	return fields;
}

bool PickStation::_check(std::string* errorMessage) {
	bool resultAll = true;
	//...
	return resultAll;
}

PluginInformation* PickStation::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<PickStation>(), &PickStation::LoadInstance);
	// ...
	return info;
}

