/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PickStation.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 21 de Junho de 2018, 19:49
 */

#include "PickStation.h"
#include "Model.h"
#include "Attribute.h"

PickStation::PickStation(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<PickStation>(), name) {

	GetterMember getter = DefineGetterMember<PickStation>(this, &PickStation::delay);
	SetterMember setter = DefineSetterMember<PickStation>(this, &PickStation::setPickStation);
	model->getControls()->insert(new SimulationControl(Util::TypeOf<PickStation>(), getName() + ".PickStation", getter, setter));

	//GetterMember getter2 = DefineGetterMember<PickStation>(this, &PickStation::delayTimeUnit);
	//SetterMember setter2 = DefineSetterMember<PickStation>(this, &PickStation::setPickStationTimeUnit);
	//model->controls()->insert(new SimulationControl(Util::TypeOf<PickStation>(), getName() + ".PickStationTimeUnit", getter2, setter2));


}

void PickStation::setPickStation(double delay) {
	_delayExpression = std::to_string(delay);
}

double PickStation::delay() const {
	return _parentModel->parseExpression(_delayExpression);
}

std::string PickStation::show() {
	return ModelComponent::show() +
			",delayExpression=" + this->_delayExpression +
			",timeUnit=" + std::to_string(static_cast<int> (this->_delayTimeUnit));
}

ModelComponent* PickStation::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	PickStation* newComponent = new PickStation(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void PickStation::setPickStationExpression(std::string _delayExpression) {
	this->_delayExpression = _delayExpression;
}

std::string PickStation::delayExpression() const {
	return _delayExpression;
}

void PickStation::setPickStationTimeUnit(Util::TimeUnit _delayTimeUnit) {
	this->_delayTimeUnit = _delayTimeUnit;
}

Util::TimeUnit PickStation::delayTimeUnit() const {
	return _delayTimeUnit;
}

void PickStation::_execute(Entity* entity) {
	double waitTime = _parentModel->parseExpression(_delayExpression);
	Util::TimeUnit stu = _parentModel->getSimulation()->getReplicationBaseTimeUnit(); //getReplicationLengthTimeUnit();
	waitTime *= Util::TimeUnitConvert(_delayTimeUnit, stu);
	if (_reportStatistics) {
		_cstatWaitTime->getStatistics()->getCollector()->addValue(waitTime);
		if (entity->getEntityType()->isReportStatistics())
			entity->getEntityType()->addGetStatisticsCollector(entity->getEntityTypeName() + ".WaitTime")->getStatistics()->getCollector()->addValue(waitTime);
	}
	entity->setAttributeValue("Entity.TotalWaitTime", entity->getAttributeValue("Entity.TotalWaitTime") + waitTime);
	double delayEndTime = _parentModel->getSimulation()->getSimulatedTime() + waitTime;
	Event* newEvent = new Event(delayEndTime, entity, this->getNextComponents()->getFrontConnection());
	_parentModel->getFutureEvents()->insert(newEvent);
	_parentModel->getTracer()->trace("End of delay of entity " + std::to_string(entity->entityNumber()) + " scheduled to time " + std::to_string(delayEndTime) + Util::StrTimeUnitShort(stu) + " (wait time " + std::to_string(waitTime) + Util::StrTimeUnitShort(stu) + ") // " + _delayExpression);
}

bool PickStation::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		this->_delayExpression = LoadField(fields, "delayExpression", DEFAULT.delayExpression);
		this->_delayTimeUnit = LoadField(fields, "delayExpressionTimeUnit", DEFAULT.delayTimeUnit);
	}
	return res;
}

void PickStation::_initBetweenReplications() {
}

std::map<std::string, std::string>* PickStation::_saveInstance() {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(); //Util::TypeOf<PickStation>());
	SaveField(fields, "delayExpression", this->_delayExpression, DEFAULT.delayExpression);
	SaveField(fields, "delayExpressionTimeUnit", _delayTimeUnit, DEFAULT.delayTimeUnit);
	return fields;
}

bool PickStation::_check(std::string* errorMessage) {
	//include attributes needed
	ElementManager* elements = _parentModel->getElements();
	std::vector<std::string> neededNames = {"Entity.TotalWaitTime"};
	std::string neededName;
	for (unsigned int i = 0; i < neededNames.size(); i++) {
		neededName = neededNames[i];
		if (elements->getElement(Util::TypeOf<Attribute>(), neededName) == nullptr) {
			Attribute* attr1 = new Attribute(_parentModel, neededName);
			elements->insert(attr1);
		}
	}
	return _parentModel->checkExpression(_delayExpression, "PickStation expression", errorMessage);
}

void PickStation::_createInternalElements() {
	if (_reportStatistics && _cstatWaitTime == nullptr) {
		_cstatWaitTime = new StatisticsCollector(_parentModel, getName() + "." + "WaitTime", this);
		_childrenElements->insert({"WaitTime", _cstatWaitTime});
		// include StatisticsCollector needed in EntityType 
		ElementManager* elements = _parentModel->getElements();
		std::list<ModelElement*>* enttypes = elements->getElementList(Util::TypeOf<EntityType>())->list();
		for (std::list<ModelElement*>::iterator it = enttypes->begin(); it != enttypes->end(); it++) {
			EntityType* enttype = static_cast<EntityType*> ((*it));
			if ((*it)->isReportStatistics())
				enttype->addGetStatisticsCollector(enttype->getName() + ".WaitTime"); // force create this CStat before simulation starts
		}
	} else {
		_removeChildrenElements();
		// \todo remove StatisticsCollector needed in EntityType
	}
}

PluginInformation* PickStation::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<PickStation>(), &PickStation::LoadInstance);
	return info;
}
