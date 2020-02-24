#ifndef COURSE_HPP
#define COURSE_HPP

#include "fsm.hpp"

struct maths_course
{
    /* vectors of necessary part of schedule */
    /* f.e. in the first day of term lecture and practice are planned */
    maths_course(std::vector<std::vector<states>> const &basic_schedule =
            {
                    /* day schedule */
                    {LECTURE,     PRACTICE},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     CONTROL_TASK},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     PRACTICE},
                    {THEORY_TEST, PRACTICE},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     CONTROL_TASK},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     PRACTICE},
                    {LECTURE,     CONTROL_TASK},
            },
                 std::vector<std::vector<states>> const &session =
                         {
                                 {CONSULTATION},
                                 {EXAM},
                                 {PASSED},
                                 {FINAL},
                         },
                 std::vector<std::vector<states>> const &additional_session =
                         {
                                 {EXAM_RETAKE},
                                 {PASSED},
                                 {FINAL},
                         },

            /* maximum total score for the state */
                 std::unordered_map<int, double> const &total_score =
                         {
                                 {PRACTICE,     13},
                                 {THEORY_TEST,  7},
                                 {CONTROL_TASK, 60},
                                 {EXAM,         20},
                         },

            /* number of exam retakes during the additional session */
                 int exam_retakes_number = 2);

    void run();

private:
    fsm const course_fsm;
    std::unordered_map<int, double> const total_score;
    int const exam_retakes_number;

    int const THREE_SCORE = 60;
    int const FOUR_SCORE = 75;
    int const FIVE_SCORE = 90;
    int const EXAM_MIN = 12;

    /* get current total score */
    double get_total_score(std::unordered_map<int, double> &current_score);

    void update_score(int current_state, std::unordered_map<int, double> &current_score,
                      std::vector<double> &tasks_grade, double result);

    /* get maximal score for one state, f.e. if there are 3 control tasks and total score is 60 than returns 20 */
    double get_max_score(int state);

    /* returns 2, 3, 4 or 5 depending on the total score */
    int get_grade(double total_score);

    double process_input(int current_state, std::unordered_map<int, double> &current_score,
                         std::vector<double> &tasks_grade, double k = 1);

    bool process_state(int &timer, int current_state, std::unordered_map<int, double> &current_score,
                       std::vector<double> &tasks_grade);

    void ask_input_result(double minimum, double maximum);

};

#endif
