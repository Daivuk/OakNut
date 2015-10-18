#pragma once
#include "glm/glm.hpp"

#include <algorithm>
#include <vector>

namespace onut
{
    /**
    Seed
    */
    void randomizeSeed();

    /**
    Get a random int
    @param max Inclusive maximum
    @param min Inclusive minimum
    */
    int randi();
    int randi(int max);
    int randi(int min, int max);

    /**
    * @fn Returns random bool. True or false
    * @param chances Percentage (0-1) of changes to return true
    */
    bool randb();
    bool randb(float chances);

    /**
    Get a random float
    @param max Inclusive maximum
    @param min Inclusive minimum
    */
    float randf(float max);
    float randf(float min, float max);

    /**
    Random on vector maths
    */
    glm::vec2 rand2f(const glm::vec2& max);
    glm::vec2 rand2f(const glm::vec2& min, const glm::vec2& max);
    glm::vec3 rand3f(const glm::vec3& max);
    glm::vec3 rand3f(const glm::vec3& min, const glm::vec3& max);
    glm::vec4 rand4f(const glm::vec4& max);
    glm::vec4 rand4f(const glm::vec4& min, const glm::vec4& max);

    /**
    Random coloring
    */
    glm::vec4 randc(float alpha = 1.f);
    glm::vec4 randc(const glm::vec4& max);
    glm::vec4 randc(const glm::vec4& min, const glm::vec4& max);
    glm::vec4 randc(const glm::vec4& color, float variation);
    glm::vec4 randc(const std::vector<glm::vec4>& palette, float variation = 0.f);

    /**
    Random pick in an array
    */
    template<typename T,
        int N>
        const T& randa(const T(&arr)[N])
    {
        auto rnd = randi(N - 1);
        return arr[rnd];
    }

    /**
    Random pick in an iteratable (Vector, list, etc)
    */
    template<typename T>
    const T& randv(const std::vector<T>& vec)
    {
        auto rnd = randi(vec.size() - 1);
        return vec[rnd];
    }

    /**
    Shuffle an array
    */
    template<typename T>
    void shuffle(std::vector<T>& vec)
    {
        std::random_shuffle(vec.begin(), vec.end(), [](int i)
        {
            return randi() % i;
        });
    }

    /**
    Templated random function
    */
    template<typename Ttype>
    Ttype randt(const Ttype& min, const Ttype& max);
    template<typename Ttype>
    Ttype randt(const Ttype& max);
}
