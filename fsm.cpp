#include "fsm.hpp"

fsm::fsm(std::vector<std::vector<states>> const &basic_schedule,
         std::vector<std::vector<states>> const &session,
         std::vector<std::vector<states>> const &additional_session,
         int const exam_retakes_number)
        :
        basic_schedule(basic_schedule), session(session), additional_session(additional_session),
        exam_retakes_number(exam_retakes_number)
{
    create_fsm();
}

void fsm::create_fsm()
{
    fsm_data.resize(FINAL - TERM_START + 1);
    state_counter.resize(FINAL - TERM_START + 1, 0);
    int timer;

    /* creation of term part of FSM */
    timer = 1;
    create_basic_transitions(timer, TERM_START, basic_schedule);
    fsm_data[basic_schedule.back().back()][timer++] = SESSION_START;
    create_zero_transition(CONTROL_TASK, TASKS_RETAKE);

    /* creation of session part of FSM */
    create_basic_transitions(timer, SESSION_START, session);
    create_zero_transition(SESSION_START, PASSED, FINAL);
    create_zero_transition(EXAM, NOT_PASSED, ADDITIONAL_SESSION_START);

    /* creation of additional session part of FSM */
    timer = get_transition(EXAM, PASSED) + 1;
    create_basic_transitions(timer, ADDITIONAL_SESSION_START, additional_session);

    for (int i = 0; i < exam_retakes_number - 1; i++)
    {
        /* addition of exam retake */
        create_zero_transition(EXAM_RETAKE, NOT_PASSED, EXAM_RETAKE);
        timer = get_transition(EXAM_RETAKE, PASSED);
        fsm_data[NOT_PASSED][timer + 1] = FINAL;
        create_basic_transitions(timer, NOT_PASSED, additional_session);
    }
}


void fsm::create_basic_transitions(int &timer, int const first_state, std::vector<std::vector<states>> const &schedule)
{
    int current_state = first_state;
    for (const auto &day_schedule : schedule)
    {
        for (auto state : day_schedule)
        {
            state_counter[state]++;
            fsm_data[current_state][timer] = state;
            current_state = state;
            timer++;
        }
    }
}

void fsm::create_zero_transition(int const from, int const to, int const state)
{
    for (auto[transition, s] : fsm_data[from])
    {
        fsm_data[to][transition] = state == -1 ? s : state;
    }
    fsm_data[from][0] = to;
}

int fsm::get_transition(int const first, int const second)
{
    int result = -1;
    for (auto[transition, state] : fsm_data[first])
    {
        if (state == second)
        {
            result = std::max(result, transition);
        }
    }
    return result;
}

states fsm::get_start_state() const
{
    return TERM_START;
}

states fsm::get_final_state() const
{
    return FINAL;
}

states fsm::next_state(states const current_state, int const transition) const
{
    return (states) fsm_data[current_state].at(transition);
}

int fsm::calc_occurrences(states const state) const
{
    return state_counter[state];
}


