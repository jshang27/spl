# DOCS
Documentation of the SPL programming language.

## Preface
The stack is a list of 64-bit values (a `long` of unspecified signage) which can be visualised as a vertical "stack" of values. The `top` of the stack refers to the most recently added item, or the top-most item of the stack. The `#top` of the stack in this documentation will often be used as a number (i.e. "`#top`th member" or "a size of `#top`") and `top` as an actual reference to the top-most value.

Some syscalls will only take the latter 32 bits of an item on the stack or the latter 16 bits.

An operation can be described with it's consumption (a number corresponding to the number of items it removes from the stack and therefore requires to run) and it's return amount: the number of items it returns. The return amount is not the net addition to the stack, rather any addition to the original stack. These are indicated as `consumption`->`return amount`. For example, an operation that simply pops the top-most item of the stack would be described as `1 -> 0`. This can be called the operation's signature. 

Some operations might have signatures that don't neatly fit within the structure: for example, `dup` requires one value to be on the stack, but doesn't necessarily consume one. These will be described as `requires, not including consumption` + `consumption` -> `return amount`. `dup` would be described as `1 + 0 -> 1`. Some might require an indeterminate amount of contents, where `any` will be used.

# Table of Contents
- [DOCS](#docs)
  - [Preface](#preface)
- [Table of Contents](#table-of-contents)
- [Stack-related operations](#stack-related-operations)
  - [GET, PEEK, \& SIZE Example](#get-peek--size-example)
  - [Strings](#strings)
    - [Escape Characters](#escape-characters)
- [Control-flow Keywords](#control-flow-keywords)
- [Funtional Keywords](#funtional-keywords)

# Stack-related operations
| Name     | Keyword | Signature  | Description 
|:--------:|:-------:|:----------:|-------------
| PUSH     |         | 0 -> 1     | Pushes a `long` to the top of the stack.
| PUSH_STR |         | 0 -> 1     | Pushes a pointer to a string (`char*`) to the top of the stack. See further at [String](#strings).
| POP      | `pop`   | 1 -> 0     | Pops the top-most item of the stack.
| DUP      | `dup`   | 1 + 0 -> 1 | Duplicates `top`.
| GET      | `get`   | any + 1 -> 1 | First, pops the `top` and stores the value. Then, duplicates and removes the `#top`th item of the stack from the position of the stack after popping the original `x`.
| PEEK     | `peek`  | any + 1 -> 1 | Same as `get` (see above), except does not remove the `#top`th item.
| SIZE     | `size`  | 0 -> 1     | Returns the size of the stack **BEFORE** this item is added.
| SWAP     | `swp`   | 2 -> 2     | Swaps the `top` and the item below `top`.
| DUMP     | `dump`  | # -> 1     | Clears the stack.

## GET, PEEK, & SIZE Example
```spl
5 4 3 2 1 0 3 get
```
After the full above line is executed, the stack would look like `5 4 2 1 0 3`. If the same line, but with `peek` instead was executed like so:
```spl
5 4 3 2 1 0 3 peek
```
The stack would look like `5 4 3 2 1 0 3`. 

Using `peek` with `size`, you can simulate regular address-based memory. For example, we can have a stack where we have a few important variables on it (in a real example, these may change from run to run):
```spl
69 420
```
Then we do a few operations that muddy the stack with values that we cannot pop, but we still need those original values. We can recopy those from the bottom of the stack by using `size`.
```spl
size 1 - peek
```
This would place `69` on the top of the stack again. The behavior of `size` and `peek` lead to the helpful property that `size X - peek` places the `X`th item (1-indexed) from the bottom on the top of the stack.

## Strings
Strings are defined using `"` double quotes. Single quotes are not valid characters and are considered parts of words. Strings may not have newlines in them, and they must be closed for proper compilation.

### Escape Characters
Escape characters are characters not normally typeable that can be placed in strings using the `\` backslash. Most valid escape characters can be found in [tokenizer.cpp](tokenizer.cpp) under the `escape_chars` map.. All valid escape characters are as follows: 
- `\n`, signifying a newline.
- `\r`, signifying a carriage return.
- `\b`, signifying a backspace.
- `\t`, signifying a tab.
- `\e`, signifying an escape character (`\033`).
- `\"`, signifying a double quote.
- `\\`, signifying a backslash.
- `\0`, signifying a null character.

# Control-flow Keywords
| Name | Keyword | Signature | Description
|:----:|:-------:|:---------:|------------
| IF   | `if`    | 1 -> 0    | Consumes `top`: if `top`'s last bit (0b0100010***1***) is on, continue; otherwise, skip to a matching `else` if defined or `end`.
| ELSE | `else`  | 0 -> 0    | If the matching `if` statement was false, execute the following code; otherwise, skip to the matching `end`.
| END  | `end`   | 0 -> 0    | Ends an `if` or `if / else` block.
| WHILE| `while` | 1 -> 0    | Consumes `top`: if `top`'s last bit is on, continue; otherwise, skip to a matching `end`.
| WEND | `end`   | 1 -> 0    | Consumes `top`: if `top`'s last bit is on, jump back to the matching `while`; otherwise, carry on.

# Funtional Keywords
An example function declaration might look like:
```spl
function add 2 -> 1
    + return
end
```
Unlike the signatures used in these docs, the non-native function signatures only contain the `requirements` -> `return values`. The type checker will assume `requirements` items have been popped no matter what.

| Name | Keyword  | Description
|:----:|:--------:|------------
| FDEF |`function`| Defines a function.
| NAME |          | A non-reserved word.
| FEND | `end`    | Ends a function block.
| ANY  | `any`    | Used for declaring a function signature, where it means an indeterminate amount.
|ARROW | `->`     | Used for declaring a function signature, where it marks the boundary between `requirements` and `return values`.
| RET  | `return` | Returns back to the calling location. Functions will automatically have a `return` inserted at `end` if one is not already there.