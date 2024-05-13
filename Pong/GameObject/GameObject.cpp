#include "GameObject.h"

// Destructor
GameObject::~GameObject() {
	// Clean up components
	for (Component* component : components) {
		delete component;
	}
}

// Member function to add a component
void GameObject::addComponent(Component* component) {
	components.push_back(component);
}

void GameObject::setController(IGameObjectController* newController) {
	controller = newController;
}
void GameObject::update(float dt) {
	if (controller) {
		controller->update(*this, dt);
	}
}