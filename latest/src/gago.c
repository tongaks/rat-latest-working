#include <stdio.h>
#include <stdbool.h>

#define not(a) (!(a))
#define and(a, b) ((a) && (b))
#define or(a, b) ((a) || (b))
#define if_then(a, b) (!(a) || (b))
#define if_and_only_if(a, b) ((a) == (b))

int main() {
    int k = 0;
    int p[4] = {1, 1, 0, 0};
    int q[4] = {1, 0, 1, 0};
    int p3[8] = {1, 1, 1, 1, 0, 0, 0, 0};
    int q3[8] = {1, 1, 0, 0, 1, 1, 0, 0};
    int r3[8] = {1, 0, 1, 0, 1, 0, 1, 0};

    printf("\nA p | q | ~p | ~q | ~p ^ ~q\n");
    for (int i = 0; i < 4; i++) {
        printf("%d %d %d %d %d\n",
            i + 1,
            p[i],
            q[i],
            not(p[i]),
            not(q[i]),
            and(not(p[i]), not(q[i])));
    }

    printf("\nB p q | ~p | ~q | ~p -> ~q\n");
    for (int i = 0; i < 4; i++) {
        printf("%d %d %d | %d %d | %d\n",
            i + 1,
            p[i],
            q[i],
            not(p[i]),
            not(q[i]),
            if_then(not(p[i]), not(q[i])));
    }

    printf("\nC p | q | p ^ q | (p ^ q) -> q\n");
    for (int i = 0; i < 4; i++) {
        printf("%d %d %d %d | %d\n",
            i + 1,
            p[i],
            q[i],
            and(p[i], q[i]),
            if_then(and(p[i], q[i]), q[i]));
    }

    printf("\nD p | q | p -> q | p ^ (p -> q) | [p ^ (p -> q)] -> q\n");
    for (int i = 0; i < 4; i++) {
        printf("%d %d %d %d %d %d\n",
            i + 1,
            p[i],
            q[i],
            if_then(p[i], q[i]),
            and(p[i], if_then(p[i], q[i])),
            if_then(and(p[i], if_then(p[i], q[i])), q[i]));
    }

    printf("\nE p | q | ~q | ~p | p -> q | ~q -> ~p | (p -> q) <=> (~q -> ~p)\n");
    for (int i = 0; i < 4; i++) {
        printf("%d %d | %d %d | %d %d | %d\n",
            i + 1,
            p[i],
            q[i],
            not(q[i]),
            not(p[i]),
            if_then(p[i], q[i]),
            if_then(not(q[i]), not(p[i])),
            if_and_only_if(if_then(p[i], q[i]), if_then(not(q[i]), not(p[i])));
    }

    printf("\nF p | q | r | p v q | ~(p v q) | (p v q) -> r | ~(p v q) -> r | [(p v q) -> r] v [~(p v q) -> r]\n");
    for (int i = 0; i < 8; i++) {
        printf("%d %d %d %d | %d %d | %d %d | %d\n",
            i + 1,
            p3[i],
            q3[i],
            r3[i],
            or(p3[i], q3[i]),
            not(or(p3[i], q3[i])),
            if_then(or(p3[i], q3[i]), r3[i]),
            if_then(not(or(p3[i], q3[i])), r3[i]),
            or(if_then(or(p3[i], q3[i]), r3[i]), if_then(not(or(p3[i], q3[i])), r3[i])));
    }

    int A = 0, B = 1, C = 0, D = 1;
    printf("\nA = %d, B = %d, C = %d, D = %d\n", A, B, C, D);
    printf("\na ~[A -> (~B <=> C)] -> [A v ~(B -> D)]\n");
    printf("answer : %d\n",
        if_then(not(if_then(A, if_and_only_if(not(B), C))), or(A, not(if_then(B, D)))));

    printf("\nb [(A v B) -> (C -> D)] <=> [(A ^ B) -> (C -> ~D)]\n");
    printf("answer : %d\n",
        if_and_only_if(if_then(or(A, B), if_then(C, D)), if_then(and(A, B), if_then(C, not(D)))));

    return 0;
  }
}