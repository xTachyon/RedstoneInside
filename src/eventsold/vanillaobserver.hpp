#ifndef REDI_EVENT_VANILLAOBSERVER
#define REDI_EVENT_VANILLAOBSERVER

#include "observer.hpp"

namespace redi
{

class VanillaObserver : public Observer
{
public:
  
  void onEnable() override ;

  void onWeatherChange(WeatherChangeEvent& event) override;
};

} // namespace redi

#endif // REDI_EVENT_VANILLAOBSERVER