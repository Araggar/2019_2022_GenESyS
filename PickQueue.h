
#ifndef PICKQUEUE_H
#define PICKQUEUE_H

#include "ModelComponent.h"
#include "List.h"
#include "Station.h"
#include "Resource.h"
#include "Queue.h"

/*!
 PickQueue module
DESCRIPTION
  Picks a Queue from a queue list
 */
class PickQueue : public ModelComponent {
public: // constructors
	PickQueue(Model* model, std::string name = "");
	virtual ~PickQueue() = default;
public: // virtual
	virtual std::string show();
public: // static
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
	void addQueue(Queue* queue);
protected: // virtual
	virtual void _execute(Entity* entity);
	virtual void _initBetweenReplications();
	virtual bool _loadInstance(std::map<std::string, std::string>* fields);
	virtual std::map<std::string, std::string>* _saveInstance();
	virtual bool _check(std::string* errorMessage);
private: // methods
private: // attributes 1:1
private: // attributes 1:n
	List<Queue*>* _list = new List<Queue*>();
};


#endif /* PickQueue_H */

