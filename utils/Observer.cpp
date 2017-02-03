#include "Observer.h"

void Entity::SetName(const std::string &name)
{
   m_entity_name = name;
}

void Broadcaster::AddObserver(Observer *dat)
{
   m_observers.push_front(dat);
}

void Broadcaster::RemoveObserver(Observer *dat)
{
   auto it = std::find(m_observers.begin(),m_observers.end(),dat);
   if(it != m_observers.end())
   {
      m_observers.erase(it);  
   }
}
  
void Broadcaster::Notify(const Entity &entity, Event event)
{
   for(const auto &cur_obs : m_observers)
   {
      cur_obs->OnNotify(entity,event);
   }   
}
