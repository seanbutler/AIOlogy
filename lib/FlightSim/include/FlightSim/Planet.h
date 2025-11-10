#ifndef PLANET_H
#define PLANET_H

struct Planet {
    // Gravitational properties
    double gravity;              // m/s^2
    
    // Atmospheric properties
    double seaLevelDensity;      // kg/m^3
    double seaLevelPressure;     // Pa
    double seaLevelTemp;         // K
    double tempLapsRate;         // K/m (temperature decrease with altitude)
    double gasConstant;          // J/(kg·K) for atmosphere
    
    // Atmospheric model type
    bool hasAtmosphere;          // Whether planet has atmosphere
    
    // Constructor with Earth defaults
    Planet()
        : gravity(9.81),
          seaLevelDensity(1.225),
          seaLevelPressure(101325.0),
          seaLevelTemp(288.15),
          tempLapsRate(0.0065),
          gasConstant(287.05),
          hasAtmosphere(true)
    {}
};

#endif // PLANET_H
