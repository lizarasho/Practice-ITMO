#ifndef FSM_HPP
#define FSM_HPP

#include "states.hpp"
#include <vector>
#include <unordered_map>

struct fsm
{
    fsm(std::vector<std::vector<states>> const &basic_schedule,
        std::vector<std::vector<states>> const &session,
        std::vector<std::vector<states>> const &additional_session,
        int exam_retakes_number);

    states get_start_state() const;

    states get_final_state() const;

    states next_state(states current_state, int transition) const;

    int calc_occurrences(states state) const;

private:
    std::vector<std::vector<states>> const basic_schedule;
    std::vector<std::vector<states>> const session;
    std::vector<std::vector<states>> const additional_session;
    int const exam_retakes_number;

    std::vector<std::unordered_map<int, int>> fsm_data;
    std::vector<int> state_counter;

    void create_fsm();

    void create_basic_transitions(int &timer, int first_state, std::vector<std::vector<states>> const &schedule);

    void create_zero_transition(int from, int to, int state = -1);

    int get_transition(int first, int second);
};

#endif
