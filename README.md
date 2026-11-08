# BF-Plus
Brainfuck but kind of better and more usable.

# Syntax
Let's look at the syntax.
You probably know the default one from Brainfuck("+-,.[]<>")
I didn't change that much but in my opinion it makes it a lot easier.
First you can now do:
```BF+
+
or
+XXX
```
XXX is a three digit number.
You may ask why only three? Because it's faster.
Same goes for -, < and >.
Another thing added is "if" and "else" which works like this:
```BF+
{XXXYYY:ZZZ}
```
XXX is the three digit cmp number(should be 0-255, everything else is useless).
YYY is all the code ran when it is true(can also be longer than three chars).
ZZZ is all the code ran when it is false.

I also added jump.
I know if + jump is like the same as a loop but still loops are shorter to do.

Example:
```BF+
+.#0000
```

\# is the jump, in this case it jumps to cmd 0 again.
So this is a permanent loop.
Which means the syntax for a jump is: #XXXX(Yes 4 digits).
I know this hard caps code length to 10000 chars,
but i don't think there ever will be anything longer than that.
