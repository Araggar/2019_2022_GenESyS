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

void PickQueue::addQueue(Queue* queue) {
	this->_listQueue->insert(queue);
}

/*
 * Smallest Number in Queue.
 * Select the QUEUE block with the current smallest number of entities resident. Break ties using the POR rule.
*/
void PickQueue::setSNQ() {
	this->selection = 0;
}

/*
 * CYClic priority. 
 * Select the first available QUEUE block starting with the successor of the last QUEUE block selected.
 */
void PickQueue::setCyc(){
	this->selection = 1;
}

/*
 * Random.
 * Select randomly from the available QUEUE blocks.
*/
void PickQueue::setRandom(){
	this->selection = 2;
}

/*
 * Preferred Order Rule.
 * Select the first available QUEUE block.
*/
void PickQueue::setPOR(){
	this->selection = 3;
}

/*
 * Largest Number in Queue.
 * Select the QUEUE block with the current largest number of entities resident. Break ties using the POR rule.
*/
void PickQueue::setLNQ(){
	this->selection = 4;
}

/*
 * Largest Remaining Capacity.
 * Select the QUEUE block with the current largest remaining capacity. Break ties using the POR rule.
*/
void PickQueue::setLRC(){
	this->selection = 5;
}

/*
 * Smallest Remaining Capacity.
 * Select the QUEUE block with the current smallest remaining capacity. Break ties using the POR rule.
*/
void PickQueue::setSRC(){
	this->selection = 6;
}

/*
 * User Rule.
 * Select the URth QUEUE block where UR is computed in a user-coded rule function.
*/
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
	//_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(this->_listQueue->getAtRank(chosen)->size()) + "\"");
	return this->_listQueue->getAtRank(chosen);
}

Queue* PickQueue::pickCyc() {
	Queue* chosen;
	chosen = this->_listQueue->getAtRank(this->currentQ);
	this->currentQ = (this->currentQ +1) % this->_listQueue->size();
	return chosen;
}

Queue* PickQueue::pickRandom() {
	SamplerDefaultImpl1 sampler = SamplerDefaultImpl1();
	unsigned int chosen = this->sampler.sampleUniform(0, this->_listQueue->size());
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(chosen) + "\"");
	return this->_listQueue->getAtRank(chosen);
}

Queue* PickQueue::pickPOR() {
	// Requires queues to have a maximum capacity
	//for (unsigned int it = 0; it < this->_listQueue->size(); it++) {
	//	if (!this->_listQueue->getAtRank(it)->full()) {
	//	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(this->_listQueue->getAtRank(it)) + "\"");
	//	return this->_listQueue->getAtRank(it);
	//	}
	//}
	return this->_listQueue->getAtRank(0);
}

Queue* PickQueue::pickLNQ() {
	unsigned int chosen = 0;
	for (unsigned int it = 1; it < this->_listQueue->size(); it++) {
		if (this->_listQueue->getAtRank(chosen)->size() < this->_listQueue->getAtRank(it)->size()) {
			chosen = it;
		}
	}
	return this->_listQueue->getAtRank(chosen);
}

Queue* PickQueue::pickLRC() {
	// Requires queues to have a maximum capacity
	//unsigned int chosen = 0;
	//for (unsigned int it = 1; it < this->_listQueue->size(); it++) {
	//	if (this->_listQueue->getAtRank(chosen)->remainingCapacity() < this->_listQueue->getAtRank(it)->remainingCapacity()) {
	//		chosen = it;
	//	}
	//}
	//return this->_listQueue->getAtRank(chosen);
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

