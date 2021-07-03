##THE ENTRYPOINT

#Problem:
The entrypoint will not be a function. Reason being that it's weird to be able to implement recursion
from the entrypoint (which is actually possible with the main function in C).

#Solution:
Since this language is roughly trying to follow the "every structure is an extension of a scope"-philosophy,
the language can have yet another kind of scope extension being the entrypoint. It cannot be called like a function. 
It just takes the arguments from the command line, bounds that to the scope with `argc` and `argv`, and starts
executing the statements from top to bottom.

One could argue that we now lose the ability to return error codes from the "main() function", but first of all:
Right now it doesn't look like that's an issue. And second: we could always give the "entrypoint-scope" some special
ability to return error values or set error flags or something.

#Result:
The syntax for this, which might change in the future, could (for now) look like this:
```
ENTRY_POINT(args: String[]) {
	-- code
}
```

