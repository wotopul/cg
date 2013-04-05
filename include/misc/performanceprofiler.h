#pragma once
#include <chrono>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <string>

namespace util
{
    namespace perftest
    {
        class PerformanceProfiler
        {
        public:
            PerformanceProfiler(std::string const& name, bool writeStats = true, std::ostream& logStream = std::cout)
                : _name(name),
                  _writeStats(writeStats),
                  _logStream(logStream),
                  _start(std::chrono::high_resolution_clock::now())
            {
                PerformanceProfiler::history.push(name);
            }

            ~PerformanceProfiler()
            {
                using namespace std::chrono;
                auto end = high_resolution_clock::now();
                auto time = duration_cast<duration<double>>(end - _start);

                PerformanceProfiler::history.pop();
                auto statsIterator = PerformanceProfiler::statistics.find(_name);

                if (statsIterator == PerformanceProfiler::statistics.end())
                {
                    PerformanceProfiler::statistics[_name] = {1, time.count()};
                }
                else
                {
                    ++(statsIterator->second.first);
                    statsIterator->second.second += time.count();
                }

                if (!PerformanceProfiler::history.empty())
                {
                    auto parent = PerformanceProfiler::history.top();
                    auto innerIterator = PerformanceProfiler::innerStatistics[parent].find(_name);

                    if (innerIterator == PerformanceProfiler::innerStatistics[parent].end())
                    {
                        PerformanceProfiler::innerStatistics[parent].insert( {_name, {1, time.count()}});
                    }
                    else
                    {
                        ++(innerIterator->second.first);
                        innerIterator->second.second += time.count();
                    }
                }

                if (_writeStats)
                {
                    auto sumStat = PerformanceProfiler::statistics[_name];

                    _logStream << "Statistics for test: \""
                               << _name
                               << "\".\nSingle execution time: "
                               << time.count()
                               << " seconds.\nSummary statistics:\n Run count: "
                               << sumStat.first
                               << "\n Summary time: "
                               << sumStat.second
                               << " seconds.\n Average time: "
                               << (time.count() / sumStat.first)
                               << " seconds.\n"
                               << "Inner tests:\n";

                    for (auto it = PerformanceProfiler::innerStatistics[_name].begin();
                            it != PerformanceProfiler::innerStatistics[_name].end(); ++it)
                    {
                        _logStream << " Name: \""
                                   << it->first
                                   << "\".\n  Executed "
                                   << it->second.first
                                   << " times.\n  Summary time: "
                                   << it->second.second
                                   << " seconds.\n  Average time: "
                                   << (it->second.second / it->second.first)
                                   << " seconds.\n";
                    }
                }
            }

        private:
            std::string _name;
            bool _writeStats;
            std::ostream& _logStream;
            std::chrono::high_resolution_clock::time_point _start;

            static std::unordered_map<std::string, std::pair<int, double>> statistics;
            static std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, double>>> innerStatistics;
            static std::stack<std::string> history;
        };

        std::unordered_map<std::string, std::pair<int, double>> PerformanceProfiler::statistics;
        std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, double>>> PerformanceProfiler::innerStatistics;
        std::stack<std::string> PerformanceProfiler::history;
    }
}
