## Лабораторная работа №2
# Построение и программная реализация алгоритма сплайн-интерполяции табличных функций

Полное условие задания можно найти [здесь](./docs/lab_02.pdf).

Понятные лекции с материалом по теме — [здесь](../lections/lection_03.pdf).

### Примерные вопросы при защите лабораторной работы:

1) Получить выражения для коэффициентов кубического сплайна, построенного на двух точках.

2) Выписать все условия для определения коэффициентов сплайна, построенного на 3-х точках.

3) Определить начальные значения прогоночных коэффициентов, если принять, что для коэффициентов сплайна справедливо $C_1=C_2$.

4) Пусть для коэффициентов $C_N , C_{N-1}$ записано соотношение $\alpha C_N+\beta C_{N-1} = \gamma$ , где $\alpha, \beta, \gamma$ — заданные числа. Получить выражение для определения коэффициента $C_N$, чтобы выполнить обратный ход прогонки.