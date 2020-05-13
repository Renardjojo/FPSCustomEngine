//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-13 - 17 h 14

#ifndef _RANDOM_H
#define _RANDOM_H

#include <cstdlib>
#include <time.h>
#include <type_traits>
#include <algorithm>

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths
{

    class Random
    {
        public:

        #pragma region constructor/destructor

        Random ()					            = delete;
        Random (const Random& other)			= delete;
        Random (Random&& other)				    = delete;
        virtual ~Random ()				        = delete;
        Random& operator=(Random const& other)  = delete;
        Random& operator=(Random && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region methods

        /**
         * @brief Init random seed with the current time
         * 
         */
        static void initSeed()
        {
            std::srand(time(NULL));
        }
        
        /**
         * @brief Initialize random number generator
         * 
         * @param seed The pseudo-random number generator is initialized using the argument passed as seed.
         */
        static void initSeed(float seed)
        {
            srand(seed);
        }

        /**
         * @brief This will generate a number from 0.0 to 1.0, inclusive.
         * 
         * @tparam floating point type 
         * @return T 
         */
        template<typename T = float> 
        auto unitValue() -> std::enable_if_t<std::is_floating_point<T>::value, T>
        {
            return static_cast <T> (rand()) / static_cast <T> (RAND_MAX);
        } 


        /**
         * @brief This will generate a number from 0 to 1, inclusive.
         * 
         * @tparam integral type 
         * @return T 
         */
        template<typename T = int> 
        auto unitValue() -> std::enable_if_t<std::is_integral<T>::value, T>
        {
            return static_cast<T>(rand() % 2);
        }

        /**
         * @brief This will generate a number from 0.0 to some arbitrary float, max:
         * 
         * @tparam float 
         * @param max 
         * @return T 
         */
        template<typename T = float>
        T ranged(T max)
        {
            return static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / max));
        }

        /**
         * @brief This will generate a number from some arbitrary min to some arbitrary max:
         * 
         * @tparam float 
         * @param min 
         * @param max 
         * @return T 
         */
        template<typename T = float>
        T ranged(T min, T max)
        {
            return min + static_cast <T> (rand()) /( static_cast <T> (RAND_MAX/(max - min)));
        }

#pragma region Cicular

        template<typename T = float>
        Vec2 circularCoordonate(Vec2 center, T range)
        {
            T randValue = static_cast<T>(rand());
            return Vec2{center.x + range * std::cos<T>(randValue), center.y + range * std::sin<T>(randValue)};
        }

        template<typename T = float>
        Vec2 unitCircularValue()
        {
            T randValue = static_cast<T>(rand());
            return Vec2{std::cos<T>(randValue), std::sin<T>(randValue)};
        }

#pragma endregion //!Cicular

#pragma region Spherique

        template<typename T = float>
        Vec3 spheriqueValue(Vec3 center, T range)
        {
            T phi = rangedRandom<T>(static_cast<T>(0.f), static_cast<T>(M_PI));
            T theta = rangedRandom<T>(static_cast<T>(0.f), static_cast<T>(M_PI));
            return Vec3{center.x + (range * std::::sin<T>(phi) * std::cos<T>(theta)),
                        center.y + (range * std::::sin<T>(phi) * std::sin<T>(theta)), 
                        center.z + (range * std::::cos<T>(phi))};
        }

        template<typename T = float>
        Vec3 unitSpheriqueCoordonate()
        {
            T phi = rangedRandom<T>(static_cast<T>(0.f), static_cast<T>(M_PI));
            T theta = rangedRandom<T>(static_cast<T>(0.f), static_cast<T>(M_PI));
            return Vec3{std::sin<T>(phi) * std::cos<T>(theta), std::::sin<T>(phi) * std::sin<T>(theta), std::::cos<T>(phi)};
        }

#pragma endregion //!Spherique

#pragma region Square

        /**
         * @brief return square coordonate
         * 
         * @tparam float 
         * @param center the center of the square
         * @param extX half saquare extension on x axis
         * @param extY half saquare extension on y axis
         * @return Vec2 
         */
        template<typename T = float>
        Vec2 squareCoordonate(Vec2 center, T extX, T extY)
        {           
            return Vec2{center.x + ranged<T>(-extX, extX), center.y + ranged<T>(-extY, extY)};
        }

        /**
         * @brief return unit sqare with value between 0 and 1
         * 
         * @tparam float 
         * @return Vec2 
         */
        template<typename T = float>
        Vec2 unitSquareValue()
        {
            return Vec2{unitValue<T>(), unitValue<T>())};
        }

#pragma endregion //!Square

#pragma region Cubique
        
        /**
         * @brief return cubique coordonate
         * 
         * @tparam float 
         * @param center the center of the square
         * @param extX half saquare extension on x axis
         * @param extY half saquare extension on y axis
         * @param extZ half saquare extension on z axis
         * @return Vec3 
         */
        template<typename T = float>
        Vec3 cubiqueCoordonate(Vec3 center, T extX, T extY,T extZ)
        {           
            return Vec3{center.x + ranged<T>(-extX, extX), center.y + ranged<T>(-extY, extY), center.y + ranged<T>(-extZ, extZ)};
        }

        /**
         * @brief return unit cubic with value between 0 and 1
         * 
         * @tparam float 
         * @return Vec3 
         */
        template<typename T = float>
        Vec3 unitCubiqueValue()
        {
            return Vec3{unitValue<T>(), unitValue<T>(), unitValue<T>())};
        }

#pragma endregion //!Cubique

        /**
         * @brief return if purcent value is respect in randome case. Inclusive test
         * 
         * @tparam float 
         * @param percent 
         * @return true 
         * @return false 
         */
        template<typename T = float>
        bool ranPercentProba(T percent)
        { 
            return rangedRandom<T>(static_cast<T>(0), static_cast<T>(100)) <= percent;
        }

        #pragma endregion //!methods
    };

} //namespace Engine::Core::Maths

#endif //_RANDOM_H