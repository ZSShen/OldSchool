### Addition

+ A is positive, B is positive
   R = A + B

+ A is positive, B is negative
   if (A > |B|)
       R = A - |B|
   else
       R = -(|B| - A)

+ A is negative, B is positive
   if (|A| > B)
       R = -(|A| - B)
   else
       R = B - |A|

+ A is negative, B is negative
   R = -(|A| + |B|)

### Subtraction

+ A is positive, B is positive
   if (A > B)
       R = A - B
   else
       R = B - A

+ A is positive, B is negative
   R = A + |B|

+ A is negative, B is positive
   R = -(|A| + B)

+ A is negative, B is negative
   if (|A| > |B|)
       R = -(|A| - |B|)
  else
       R = (|B| - |A|)
