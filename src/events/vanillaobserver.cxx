#include "eventmanager.hpp"
#include "vanillaobserver.hpp"

namespace redi
{

void VanillaObserver::onWeatherChange(WeatherChangeEvent&)
{
}

void VanillaObserver::onEnable()
{
  manager->registerEvent(this, EventType::WeatherChange);
}
  
} // namespace redi