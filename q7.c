/*
Write a program that creates a child process, and then in the child
closes standard output (STDOUT FILENO). What happens if the child
calls printf() to print some output after closing the descriptor?
*/