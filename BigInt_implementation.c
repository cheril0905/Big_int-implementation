#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIGITS 310

void removeLeadingZeros(char ans[]) {
    int i = 0;
    while (ans[i] == '0' && ans[i + 1] != '\0') {
        i++;
    }

    int j = 0;
    while (ans[i] != '\0') {
        ans[j++] = ans[i++];
    }
    ans[j] = '\0';
}

int isNegative(char num[]) {
    return num[0] == '-';
}

void reverseString(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void addBigInt(char num1[], char num2[], char ans[]) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int carry = 0, sum;

    reverseString(num1);
    reverseString(num2);

    int i;
    for (i = 0; i < len1 || i < len2 || carry; i++) {
        int digit1 = (i < len1 && num1[i] != '-') ? num1[i] - '0' : 0;
        int digit2 = (i < len2 && num2[i] != '-') ? num2[i] - '0' : 0;

        sum = digit1 + digit2 + carry;
        ans[i] = (sum % 10) + '0';
        carry = sum / 10;
    }
    ans[i] = '\0';

    reverseString(ans);
    reverseString(num1);
    reverseString(num2);
}

void subtractBigInt(char num1[], char num2[], char ans[]) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int borrow = 0, diff;

    reverseString(num1);
    reverseString(num2);

    int i;
    for (i = 0; i < len1; i++) {
        int digit1 = (num1[i] != '-') ? num1[i] - '0' : 0;
        int digit2 = (i < len2 && num2[i] != '-') ? num2[i] - '0' : 0;

        diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        ans[i] = diff + '0';
    }

    ans[i] = '\0';

    reverseString(ans);
    reverseString(num1);
    reverseString(num2);
    removeLeadingZeros(ans);
}

void multiplyBigInt(char num1[], char num2[], char ans[]) {
    int len1 = strlen(num1) - isNegative(num1);
    int len2 = strlen(num2) - isNegative(num2);
    int product[MAX_DIGITS * 2] = {0};
    int isNegativeResult = (isNegative(num1) != isNegative(num2));

    char absNum1[MAX_DIGITS], absNum2[MAX_DIGITS];
    strcpy(absNum1, isNegative(num1) ? num1 + 1 : num1);
    strcpy(absNum2, isNegative(num2) ? num2 + 1 : num2);

    reverseString(absNum1);
    reverseString(absNum2);

    for (int i = 0; i < len1; i++) {
        int digit1 = absNum1[i] - '0';
        for (int j = 0; j < len2; j++) {
            int digit2 = absNum2[j] - '0';
            product[i + j] += digit1 * digit2;
            if (product[i + j] >= 10) {
                product[i + j + 1] += product[i + j] / 10;
                product[i + j] %= 10;
            }
        }
    }

    int len = len1 + len2;
    while (len > 1 && product[len - 1] == 0) {
        len--;
    }

    for (int i = 0; i < len; i++) {
        ans[len - i - 1] = product[i] + '0';
    }
    ans[len] = '\0';

    removeLeadingZeros(ans);

    if (isNegativeResult && strcmp(ans, "0") != 0) {
        int len = strlen(ans);
        for (int i = len; i >= 0; i--) {
            ans[i + 1] = ans[i];
        }
        ans[0] = '-';
    }
}

void handleAddition(char num1[], char num2[], char ans[]) {
    if (isNegative(num1) && isNegative(num2)) {
        addBigInt(num1 + 1, num2 + 1, ans);
        int len = strlen(ans);
        for (int i = len; i >= 0; i--) {
            ans[i + 1] = ans[i];
        }
        ans[0] = '-';
    } else if (isNegative(num1)) {
        
        if (strcmp(num2, num1 + 1) >= 0) {
            subtractBigInt(num2, num1 + 1, ans);
        } else {
            subtractBigInt(num1 + 1, num2, ans);
            int len = strlen(ans);
            for (int i = len; i >= 0; i--) {
                ans[i + 1] = ans[i];
            }
            ans[0] = '-';
        }
    } else if (isNegative(num2)) {
        
        if (strcmp(num1, num2 + 1) >= 0) {
            subtractBigInt(num1, num2 + 1, ans);
        } else {
            subtractBigInt(num2 + 1, num1, ans);
            int len = strlen(ans);
            for (int i = len; i >= 0; i--) {
                ans[i + 1] = ans[i];
            }
            ans[0] = '-';
        }
    } else {
        addBigInt(num1, num2, ans);
    }
}

void handleSubtraction(char num1[], char num2[], char ans[]) {
    int num1IsNegative = isNegative(num1);
    int num2IsNegative = isNegative(num2);

    char absNum1[MAX_DIGITS], absNum2[MAX_DIGITS];
    strcpy(absNum1, num1IsNegative ? num1 + 1 : num1);
    strcpy(absNum2, num2IsNegative ? num2 + 1 : num2);

    if (num1IsNegative && num2IsNegative) {
        
        if (strcmp(absNum1, absNum2) > 0) {
            subtractBigInt(absNum1, absNum2, ans);
            // Result is negative
            int len = strlen(ans);
            for (int i = len; i >= 0; i--) {
                ans[i + 1] = ans[i];
            }
            ans[0] = '-';
        } else if (strcmp(absNum1, absNum2) < 0) {
            subtractBigInt(absNum2, absNum1, ans);
            // Result is positive
        } else {
            strcpy(ans, "0");
        }
    } else if (num1IsNegative) {
        
        addBigInt(absNum1, absNum2, ans);
        // Result is negative
        int len = strlen(ans);
        for (int i = len; i >= 0; i--) {
            ans[i + 1] = ans[i];
        }
        ans[0] = '-';
    } else if (num2IsNegative) {
        
        addBigInt(absNum1, absNum2, ans);
    } else {
        
        if (strcmp(absNum1, absNum2) >= 0) {
            subtractBigInt(absNum1, absNum2, ans);
        } else {
            subtractBigInt(absNum2, absNum1, ans);
            // Result is negative
            int len = strlen(ans);
            for (int i = len; i >= 0; i--) {
                ans[i + 1] = ans[i];
            }
            ans[0] = '-';
        }
    }
    removeLeadingZeros(ans);
}

int main() {
    char num1[MAX_DIGITS], num2[MAX_DIGITS], ans[MAX_DIGITS * 2];

    printf("Enter the first number: ");
    scanf("%s", num1);

    printf("Enter the second number: ");
    scanf("%s", num2);

    memset(ans, 0, sizeof(ans));
    handleAddition(num1, num2, ans);
    printf("Addition Result: %s\n", ans);

    memset(ans, 0, sizeof(ans));
    handleSubtraction(num1, num2, ans);
    printf("Subtraction Result: %s\n", ans);

    memset(ans, 0, sizeof(ans));
    multiplyBigInt(num1, num2, ans);
    printf("Multiplication Result: %s\n", ans);

    return 0;
}
