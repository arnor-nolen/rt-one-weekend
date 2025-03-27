#ifndef MAP_REDUCE_HPP
#define MAP_REDUCE_HPP

#include <future>
#include <ranges>
#include <thread>
#include <vector>

// Currently not used, needs rework.
template <typename F1, typename F2>
auto mapReduce(std::ranges::input_range auto range,
               const F1 &funcCalc,
               const F2 &funcReduce,
               const size_t threads) -> decltype(funcCalc(*range.begin()))
{

    using returnType = decltype(funcCalc(*range.begin()));

    const size_t threadDistance = range.size() / threads;
    auto promises = std::vector<std::future<returnType>>{};

    const auto chunkMapReduce
        = [&funcCalc, &funcReduce](std::ranges::input_range auto chunkRange)
    {
        auto result = funcCalc(*chunkRange.begin());
        for (const auto &elem : chunkRange)
        {
            result = funcReduce(result, funcCalc(elem));
        }
        return result;
    };

    auto threadStart = range.begin();

    for (size_t threadId = 0u; threadId < threads - 1; ++threadId)
    {
        promises.emplace_back(std::async(
            std::launch::async,
            chunkMapReduce,
            std::ranges::subrange(threadStart, threadStart + threadDistance)));
        threadStart += threadDistance;
    }
    promises.emplace_back(
        std::async(std::launch::async,
                   chunkMapReduce,
                   std::ranges::subrange(threadStart, range.end())));

    returnType result(promises[0].get());
    for (size_t i = 1; i != promises.size(); ++i)
    {
        result = funcReduce(result, promises[i].get());
    }
    return result;
}

template <typename F1>
void parallelizeForEach(std::ranges::input_range auto range,
                        const F1 &funcCalc,
                        const size_t threads)
{

    const auto chunkMapReduce = [&](std::ranges::input_range auto chunkRange)
    {
        for (const auto &elem : chunkRange)
        {
            funcCalc(elem);
        }
    };

    const auto spawnJthread = [&](std::ranges::input_range auto subrange)
    { return std::jthread{ chunkMapReduce, subrange }; };

    // Figure out how to split into evenly sized bins.
    range | std::views::chunk(range.size() / threads)
        | std::views::transform(spawnJthread) | std::ranges::to<std::vector>();
}

#endif
