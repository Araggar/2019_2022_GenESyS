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

void PickQueue::addQueue(Queue* queue) {
  this->_list->setAtRank(queue, this->_list->size() + 1);
}

Queue* PickQueue::pickQueue() {
	unsigned int chosen = 0;
  // Picks the Queue With the Smaller Size
	for (unsigned int it = 1; it < this->_list->size(); it++) {
		if (this->_list->getAtRank(chosen)->size() > this->_list->getAtRank(it)->size()) {
			chosen = it;
		}
	}
	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Queue \"" + std::to_string(this->_list->getAtRank(chosen)->size()) + "\"");
	return this->_list->getAtRank(chosen);
}

void PickQueue::_execute(Entity* entity) {
	assert(!_list->empty());
	Queue* chosen;

	chosen = pickQueue();

	_parentModel->getTracer()->trace(Util::TraceLevel::L5_arrival, "Selected " + chosen->getName());

  // TODO: Decide what to do with the Picked Queue
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
	return info;
}

