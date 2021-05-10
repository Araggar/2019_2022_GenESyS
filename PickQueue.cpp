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
	sampler = SamplerDefaultImpl1();
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

void PickQueue::addQueue(Queue* queue) {
	this->_listQueue->insert(queue);
}

void PickQueue::setSNQ() {
	this->selection = 0;
}
void PickQueue::setCyc(){
	this->selection = 1;
}
void PickQueue::setRandom(){
	this->selection = 2;
}
void PickQueue::setPOR(){
	this->selection = 3;
}
void PickQueue::setLNQ(){
	this->selection = 4;
}
void PickQueue::setLRC(){
	this->selection = 5;
}
void PickQueue::setSRC(){
	this->selection = 6;
}
void PickQueue::setExpression(){
	this->selection = 7;
}

Queue* PickQueue::pickSNQ() {
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_listQueue->size(); it++) {
		if (this->_listQueue->getAtRank(chosen)->size() > this->_listQueue->getAtRank(it)->size()) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(this->_listQueue->getAtRank(chosen)->size()) + "\"");
	return this->_listQueue->getAtRank(chosen);
}

Queue* PickQueue::pickCyc() {
	Queue* chosen;
	chosen = this->_listQueue->getAtRank(this->currentQ);
	this->currentQ = (this->currentQ +1) % this->_listQueue->size();
	//chosen = this->_listQueue->getAtRank(0);
	return chosen;
}

Queue* PickQueue::pickRandom() {
	SamplerDefaultImpl1 sampler = SamplerDefaultImpl1();
	unsigned int chosen = this->sampler.sampleUniform(0, this->_listQueue->size());
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(chosen) + "\"");
	return this->_listQueue->getAtRank(chosen);
}

Queue* PickQueue::pickPOR() {
	return this->_listQueue->getAtRank(0);
}

Queue* PickQueue::pickLNQ() {
	return this->_listQueue->getAtRank(0);
}

Queue* PickQueue::pickLRC() {
	return this->_listQueue->getAtRank(0);
}

Queue* PickQueue::pickSRC() {
	return this->_listQueue->getAtRank(0);
}

Queue* PickQueue::pickExpression() {
	return this->_listQueue->getAtRank(0);
}

void PickQueue::_execute(Entity* entity) {
	assert (!this->_listQueue->empty());
	
	Queue* chosen;
	switch (this->selection) {
		case 0:
			chosen = pickSNQ();
			break;
		case 1:
			chosen = pickCyc();
			break;
		case 2:
			chosen = pickRandom();
			break;
		case 3:
			chosen = pickPOR();
			break;
		case 4:
			chosen = pickLNQ();
			break;
		case 5:
			chosen = pickLRC();
			break;
		case 6:
			chosen = pickSRC();
			break;
		case 7:
			chosen = pickExpression();
			break;
	}

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected na");
	//this->_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
	//this->_parentModel->sendEntityToComponent(entity, chosen->getEnterIntoStationComponent(), this->_delay);
	//_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection(), 0.0);
	Waiting* waiting = new Waiting(entity, this, _parentModel->getSimulation()->getSimulatedTime());
	chosen->insertElement(waiting);


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

