#ifndef OBSERVER_H__
#define OBSERVER_H__

#include <string>
#include <list>

enum class Event{ ENTITY_KILLED, ENTITY_OTHER};

class Entity
{
   public:
   Entity::Entity(){}
   void SetName(const std::string &name);
   
   private:
   std::string m_entity_name;
};

class Observer
{
   public:
   virtual ~Observer(){}
   virtual void OnNotify(const Entity& entity, Event event) = 0;
};

class Broadcaster
{
   public:
   void AddObserver(Observer *dat);   
   void RemoveObserver(Observer *dat);
     
   protected:
   void Notify(const Entity &entity, Event event);
    
   private:
   std::list<Observer *> m_observers;
};

/* Example
class BigBrother : public Observer
{
   public:
   virtual void OnNotify(const Entity& entity, Event event)
   {
      switch(event)
      {
         case Event::ENTITY_KILLED:
         std::cout << "Another one bites the dust\n";
         break;
      }
   }
};

class StatBoy : public Observer
{
   public:
   virtual void OnNotify(const Entity& entity, Event event)
   {
      switch(event)
      {
         case Event::ENTITY_KILLED:
           m_n_killed++;
           std::cout << "n_killed = " << m_n_killed << "\n";
         break;
      }
   }
   private:
   int m_n_killed = 0;
};

class TownCrier : public Broadcaster
{
   public:
   void UpdateEntity(Entity &entity)
   {
      Notify(entity,Event::ENTITY_KILLED);
   }
};

*/

#endif