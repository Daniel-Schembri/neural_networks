#ifndef ANIMAL_HPP
#define ANIMAL_HPP 

class Animal
{
    public:
        Animal();
        void increaseHP(const double& factor);
        void decreaseHP(const double& factor);

    private:
        double m_HP;
        double m_velocity;
        Net m_Net;

        void move(const double &angle) const;
        //TODO: Sensoren (Werte direkt als Inputneuronen)




};
#endif /* ANIMAL_HPP */
