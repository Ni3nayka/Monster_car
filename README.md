# Monster_car
robot on 6-motors platform with suspension

### описание ветки:
В этой ветке экспериментировалось управление роботом с помощью дешевых радиомодулей на 433 мгц:

https://amperkot.ru/msk/catalog/radiomoduli_433mhz_peredatchik_mxfs03v_i_priemnik_mx05v-23869814.html

для этого модуля были опробованы несколько библиотек, лучшей (по API и скорости работы) была выбрана:

https://lesson.iarduino.ru/page/urok-26-6-soedinyaem-dve-arduino-po-radiokanalu-433-mgc/

В пульте от коптера вся эектроника была заменена, контроллером была arduino nano. В результате максимальное расстояние по прямой видимости, на которой робот реагировал на пульт была где-то 6-7 метров с антенной, без нее 2-3 метра. Также были задержки в передаче (около 0,2-0,4 секунд по ощущению). Результат неудовлетворителен для управления роботом

### for olymp robofinist RTK:
youtube link:
https://youtu.be/6edtZWzomFI
