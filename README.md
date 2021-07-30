# Homebrew Libraries Kit - Events

## Changelog:


<details>
<summary>v1.0.3</summary>

- Fixed isValid() check without context

</details>



<details>
<summary>v1.0.2</summary>

- Now the Delegate class stores the wrapper as std::shared_ptr<...>
- Move/Copy operators and constructors added to all wrappers
- Move/Copy operators and constructors added to the Delegate class

</details>



<details>
<summary>v1.0.1</summary>

- Added copy and move constructors to the Event
- Added NotifierObject class to watch event handler objects

</details>



<details>
<summary>v1.0.0</summary>

- First commit :)

</details>

***

## To Do:
1. Readme
2. Tests

## Known issues:
1. Delegate with lambdas may occur undefined behaviour
