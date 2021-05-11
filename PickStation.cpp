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
	op = std::greater<int>();
}

void PickStation::setMinimum() {
	op = std::less<int>();
}

void PickStation::addStation(Station* station) {
	this->_list->insert(station);
}

void PickStation::addStation(Station* station, Resource* resource) {
	setSelectionResource();
	this->_list->insert(station);
	this->_listResource->insert(resource);
}

void PickStation::addStation(Station* station, Queue* queue) {
	setSelectionQueue();
	this->_list->insert(station);
	this->_listQueue->insert(queue);
}

void PickStation::addStation(Station* station, std::string expression) {
	setSelectionExpression();
	this->_list->insert(station);
	this->_listExpression->insert(expression);
}

void PickStation::setSelectionEnRoute() {
	this->selection = 1;
}

void PickStation::setSelectionResource() {
	this->selection = 2;
}

void PickStation::setSelectionQueue() {
	this->selection = 4;
}

void PickStation::setSelectionExpression() {
	this->selection = 8;
}

void PickStation::setTimeDelay(double delay) {
	this->_delay = delay;
}

Station* PickStation::pickEnRoute() {
	Station* chosen = this->_list->getAtRank(0);
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		//if (chosen->getNumberInStation() > this->_list->getAtRank(it)->getNumberInStation()) {
		if (op(this->_list->getAtRank(it)->getNumberInStation(), chosen->getNumberInStation())) {
			chosen = this->_list->getAtRank(it);
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "'EnRoute' Sent to \"" + chosen->getName() + "\"");
	return chosen;
}

Station* PickStation::pickResource() {
	assert (this->_list->size() == this->_listResource->size());
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		//if (this->_listResource->getAtRank(chosen)->getNumberBusy() > this->_listResource->getAtRank(it)->getNumberBusy()) {
		if (op(this->_listResource->getAtRank(it)->getNumberBusy(), this->_listResource->getAtRank(chosen)->getNumberBusy())) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "'Resource' Sent to \"" + this->_list->getAtRank(chosen)->getName() + "\"");
	return this->_list->getAtRank(chosen);
}

Station* PickStation::pickQueue() {
	assert (this->_list->size() == this->_listQueue->size());
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		//if (this->_listQueue->getAtRank(chosen)->size() > this->_listQueue->getAtRank(it)->size()) {
		if (op(this->_listQueue->getAtRank(it)->size(), this->_listQueue->getAtRank(chosen)->size())) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "'Queue' Sent to \"" + this->_list->getAtRank(chosen)->getName() + "\"");
	return this->_list->getAtRank(chosen);
}

Station* PickStation::pickExpression() {
	assert (this->_list->size() == this->_listExpression->size());
	for (unsigned int it = 0; it < this->_list->size(); it++) {
		if (_parentModel->parseExpression(this->_listExpression->getAtRank(it))) {
			_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "'Expression' Sent to \"" + this->_list->getAtRank(it)->getName() + "\"");
			return this->_list->getAtRank(it);
		}
	}
}

void PickStation::_execute(Entity* entity) {
	assert(!_list->empty());
	Station* chosen;

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected " + std::to_string(this->selection));
	switch (this->selection) {
		case 1 :
			chosen = pickEnRoute();
			break;
		case 2 :
			chosen = pickResource();
			break;
		case 4 :
			chosen = pickQueue();
			//chosen = pickExpression();
			break;
		case 8:
			chosen = pickExpression();
			break;
	}

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected " + chosen->getName());
	this->_parentModel->sendEntityToComponent(entity, chosen->getEnterIntoStationComponent(), this->_delay);
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
	info->setSendTransfer(true);
	// ...
	return info;
}

