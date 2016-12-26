#include <iostream>
#include "eventmanager.hpp"
#include "vanillaobserver.hpp"

namespace redi
{

void VanillaObserver::onWeatherChange(WeatherChangeEvent& event)
{
  std::cout << &event;
}

void VanillaObserver::onEnable()
{
  manager->registerEvent(this, EventType::WeatherChange);
}
  
} // namespace redi