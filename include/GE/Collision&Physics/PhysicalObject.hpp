//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef _PHYSICAL_OBJECT_H
#define _PHYSICAL_OBJECT_H

namespace Engine::Physics
{
    class PhysicalObject
    {
        public:

        #pragma region constructor/destructor

        PhysicalObject ();
        PhysicalObject (const PhysicalObject& other);
        PhysicalObject (PhysicalObject&& other);
        virtual ~PhysicalObject ();
        PhysicalObject& operator=(PhysicalObject const& other);
        PhysicalObject& operator=(PhysicalObject && other);

        #pragma endregion //!constructor/destructor

        #pragma region methods
        #pragma endregion //!methods

        #pragma region accessor
        #pragma endregion //!accessor

        #pragma region mutator
        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor

        protected:

        #pragma region attribut

        float mass;

        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        private:

    };
}

#endif //_PHYSICAL_OBJECT_H