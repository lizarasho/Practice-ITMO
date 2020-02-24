#include "maths_course.hpp"
#include <iostream>

maths_course::maths_course(const std::vector<std::vector<states>> &basic_schedule,
                           const std::vector<std::vector<states>> &session,
                           const std::vector<std::vector<states>> &additional_session,
                           const std::unordered_map<int, double> &total_score,
                           int const exam_retakes_number)
        :
        course_fsm(basic_schedule, session, additional_session, exam_retakes_number),
        total_score(total_score),
        exam_retakes_number(exam_retakes_number)
{}

void maths_course::run()
{
    std::vector<double> tasks_grade;
    std::unordered_map<int, double> current_score;
    int timer = 1;

    states current_state = course_fsm.get_start_state();
    states final_state = course_fsm.get_final_state();

    while (current_state != final_state)
    {
        int k = process_state(timer, current_state, current_score, tasks_grade);

        int transition = k * timer;
        current_state = course_fsm.next_state(current_state, transition);

        if (transition != 0)
            timer++;
    }
    process_state(timer, final_state, current_score, tasks_grade);
}

double maths_course::get_max_score(int const state)
{
    return (double) total_score.at(state) / course_fsm.calc_occurrences((states) state);
}

double maths_course::get_total_score(std::unordered_map<int, double> &current_score)
{
    double result = 0;
    for (auto[state, score] : current_score)
    {
        if (state == THEORY_TEST && current_score[EXAM] == 0)
            continue;
        result += score;
    }
    return result;

}

int maths_course::get_grade(double const total_score)
{
    if (total_score < THREE_SCORE) return 2;
    if (total_score >= THREE_SCORE && total_score < FOUR_SCORE) return 3;
    if (total_score >= FOUR_SCORE && total_score < FIVE_SCORE) return 4;
    return 5;
}


void maths_course::update_score(int const current_state, std::unordered_map<int, double> &current_score,
                                std::vector<double> &tasks_grade,
                                double const result)
{
    switch (current_state)
    {
        case PRACTICE :
        case THEORY_TEST :
        case EXAM :
        {
            current_score[current_state] += result;
            break;
        }
        case EXAM_RETAKE :
        {
            current_score[EXAM] = std::max(current_score[EXAM], result);
            break;
        }
        case CONTROL_TASK :
        {
            tasks_grade.push_back(result);
            current_score[current_state] += result;
            break;
        }
        case TASKS_RETAKE :
        {
            if (result > tasks_grade.back())
            {
                current_score[CONTROL_TASK] += result - tasks_grade.back();
                tasks_grade.back() = result;
            }
            break;
        }
        default:
            break;
    }

}

double maths_course::process_input(int const current_state, std::unordered_map<int, double> &current_score,
                                   std::vector<double> &tasks_grade, double const k)
{
    double result;
    std::cin >> result;
    std::cout << std::endl;
    update_score(current_state, current_score, tasks_grade, k * result);
    return result;
}

bool maths_course::process_state(int &timer, int const current_state, std::unordered_map<int, double> &current_score,
                                 std::vector<double> &tasks_grade)
{
    switch (current_state)
    {
        case TERM_START :
        {
            std::cout << "— Welcome to mathematical analysis course!" << std::endl << std::endl;
            break;
        }
        case LECTURE :
        {
            std::cout << "— Lecture time" << std::endl << std::endl;
            break;
        }
        case PRACTICE :
        {
            std::cout << "— Practice time" << std::endl;
            std::cout << "Input 1 - visited practice, 0 - not visited: ";
            process_input(current_state, current_score, tasks_grade, get_max_score(current_state));
            break;
        }
        case THEORY_TEST :
        {
            std::cout << "— Theory test" << std::endl;
            ask_input_result(0, get_max_score(current_state));
            process_input(current_state, current_score, tasks_grade);
            break;
        }
        case CONTROL_TASK :
        {
            std::cout << "— Control task" << std::endl;
            ask_input_result(0, get_max_score(current_state));
            return process_input(current_state, current_score, tasks_grade) == get_max_score(CONTROL_TASK);
        }
        case TASKS_RETAKE :
        {
            std::cout << "You score for the last control task could be better and you're able to retake it."
                      << std::endl;
            ask_input_result(0, get_max_score(CONTROL_TASK));
            process_input(current_state, current_score, tasks_grade);
            break;
        }
        case SESSION_START :
        {
            std::cout << "— Session started" << std::endl;
            auto total_score = get_total_score(current_score);
            std::cout << "Your current total score is " << total_score << ". " << std::endl;
            if (get_grade(total_score) == 2)
            {
                std::cout << std::endl;
                break;
            }
            auto grade = get_grade(get_total_score(current_score));
            std::cout << "It is enough to get grade " << grade << " and not to pass exam." << std::endl;
            std::cout << "Input 1 - you are going to pass exam, 0 - you are satisfied with grade " << grade << ": ";
            return (process_input(current_state, current_score, tasks_grade) == 1);
        }
        case CONSULTATION :
        {
            std::cout << "— Consultation before exam" << std::endl << std::endl;
            break;
        }
        case EXAM :
        {
            std::cout << "— Exam" << std::endl;
            ask_input_result(EXAM_MIN, get_max_score(EXAM));
            process_input(current_state, current_score, tasks_grade);
            return (get_grade(get_total_score(current_score)) >= 3);
        }
        case ADDITIONAL_SESSION_START :
        {
            std::cout << "— Additional session time. You'll have " << exam_retakes_number << " exam retakes."
                      << std::endl << std::endl;
            break;
        }
        case EXAM_RETAKE :
        {
            std::cout << "— Exam retake" << std::endl;
            ask_input_result(EXAM_MIN, get_max_score(EXAM));
            process_input(current_state, current_score, tasks_grade);
            return (get_grade(get_total_score(current_score)) >= 3);
        }
        case FINAL :
        {
            auto total_score = get_total_score(current_score);
            auto grade = get_grade(total_score);
            std::cout << "— Mathematical analysis course is over. Your total score is "
                      << total_score << " and your grade is " << grade << "." << std::endl;
            std::cout << (grade == 2 ? "Your result is not enough to continue your study in university, sorry."
                                     : "You successfully passed these course.");
        }
        default:
            break;
    }
    return true;
}

void maths_course::ask_input_result(double const minimum, double const maximum)
{
    std::cout << "Input your result (minimum - " << minimum << ", maximum - " << maximum << "): ";
}
