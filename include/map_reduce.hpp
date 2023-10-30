#ifndef MAP_REDUCE_HPP
#define MAP_REDUCE_HPP

#include <future>
#include <ranges>
#include <thread>
#include <vector>

template <typename F1, typename F2>
auto mapReduce(std::ranges::input_range auto range, const F1 &funcCalc,
               const F2 &funcReduce, const size_t threads)
    -> decltype(funcCalc(*range.begin())) {

    using returnType = decltype(funcCalc(*range.begin()));

    const size_t threadDistance = range.size() / threads;
    auto promises = std::vector<std::future<returnType>>{};

    const auto chunkMapReduce =
        [&funcCalc, &funcReduce](std::ranges::input_range auto chunkRange) {
            auto result = funcCalc(*chunkRange.begin());
            for (const auto &elem : chunkRange) {
                result = funcReduce(result, funcCalc(elem));
            }
            return result;
        };

    auto threadStart = range.begin();

    for (size_t threadId = 0u; threadId < threads - 1; ++threadId) {
        promises.emplace_back(std::async(
            std::launch::async, chunkMapReduce,
            std::ranges::subrange(threadStart, threadStart + threadDistance)));
        threadStart += threadDistance;
    }
    promises.emplace_back(
        std::async(std::launch::async, chunkMapReduce,
                   std::ranges::subrange(threadStart, range.end())));

    returnType result(promises[0].get());
    for (size_t i = 1; i != promises.size(); ++i) {
        result = funcReduce(result, promises[i].get());
    }
    return result;
}

template <typename F1>
void parallelize(std::ranges::input_range auto range, const F1 &funcCalc,
                 const size_t threads) {

    const size_t threadDistance = range.size() / threads;
    auto promises = std::vector<std::future<void>>{};

    const auto chunkMapReduce =
        [&funcCalc](std::ranges::input_range auto chunkRange) {
            for (const auto &elem : chunkRange) {
                funcCalc(elem);
            }
        };

    auto threadStart = range.begin();

    for (size_t threadId = 0u; threadId < threads - 1; ++threadId) {
        promises.emplace_back(std::async(
            std::launch::async, chunkMapReduce,
            std::ranges::subrange(threadStart, threadStart + threadDistance)));
        threadStart += threadDistance;
    }
    promises.emplace_back(
        std::async(std::launch::async, chunkMapReduce,
                   std::ranges::subrange(threadStart, range.end())));

    for (auto &promise : promises) {
        promise.get();
    }
}

#endif
