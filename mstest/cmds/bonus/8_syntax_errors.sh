# **************************************************************************** #
#                                 SYNTAX ERRORS                                #
# **************************************************************************** #

/bin/echo 1 && /bin/echo 2 ||

/bin/echo 1 && /bin/echo 2 |

/bin/echo 1 && /bin/echo 2 &&

()

(/bin/echo 1

/bin/echo 1)

(/bin/echo 1))

(/bin/echo 1) (/bin/echo 2)

()

(echo) hi

(echo) (hi)

(echo hi && ((echo hi && (echo hi) && echo hi)))

)(

((

))

&

&&

||

||&&

echo a | &&

&& echo a

echo a &&

(&&)

&&&

&&&&

()&&()

echo a && ()echo b

)echo a(

echo () a
