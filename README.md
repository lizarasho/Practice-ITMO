### Курс по математическому анализу на основе FSM

 * **Содержание курса по умолчанию**
     * По умолчанию в курс семестра входят лекции, практики, один теоропрос, контрольные работы и их переписывания. Также на сессии запланированы консультация и экзамен, а на дополнительной сессии — 2 пересдачи.

 * **Распределение баллов по умолчанию**
     * посещение практик — `[0, 13]` баллов
     * теоропрос в середине семестра — `[0, 7]` баллов
     * сумма за 3 контрольные — `[0, 60]` баллов 
     * экзамен — `[12, 20]` баллов
 
При этом баллы за теоропрос учитываются только при успешной сдачи экзамена. 

Разбалловку, расписание семестра, основной сессии или доп.сессии и количество пересдач можно менять (задаются параметрами конструктора `maths_course`). Баллы за одну контрольную рассчитываются в зависимости от их количества в расписании, аналогично с практиками и т.д.

 * **FSM**
 
 По заданным массивам расписаний, разбалловке и количеству пересдач строится FSM. 
 
Для дефолтных параметров будет выглядеть так:
![FSM diagram](https://github.com/lizarasho/Practice-ITMO/blob/master/FSM%20diagram.jpeg)
 

Все переходы в нем основаны на значении таймера, либо на введённых данных. 

`Таймер` — по сути счётчик пройденных обязательных состояний, который заведён с целью понимать, в каком моменте расписания мы находимся. 
Например, после лекции, во время который таймер равен 8, мы перейдём в состояние КР, а по таймеру 2 — в практику. Изначально равен единице.

`0` — специальный переход для состояний, в которых таймер не меняется. Например, после написания КР не на полный балл предлагается одно переписывание. Но поскольку оно необязательно (зависит от балла за КР) и не входит в базовое расписание, то после него мы должны вернуться к привычному расписанию. 

После ненулевого перехода таймер увеличивается на 1.

Таким образом, после любого состояния при текущем значении таймера `t` функция перехода будет равна `k*t`, где `k ∈ {0, 1}` и зависит от состояния. 

 * Для состояний
     * `Control Task`: `k` ~ КР написана на полный балл
     * `Session Start`:  `k` ~ у студента недостаточно баллов на тройку без экзамена или студент хочет сдавать экзамен 
     * `Exam`, `Exam retake`: `k` ~ студент набрал необходимый суммарный минимум на тройку

По умолчанию `k = 1`.

В итоге любая последовательность переходов от старта до финала, допускаемая автоматом, будет иметь вид `1, 2, 3, ... x, 0, x + 1, ..., n - 1, n` (подотрезок натуральных чисел, между которыми иногда могут «вклиниваться» нули).

 * **Реализация**
 
 Основное действие выполняет класс `maths_course` с одним публичным методом `run()`. 
 
 Внутри себя хранит экземпляр класса `fsm`, который по заданному расписанию и количеству пересдач строит нужный автомат и позволяет переходить между состояниями.
