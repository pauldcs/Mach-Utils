## Example usage

```C
// spawns the program, the process starts in stopped state
if (!spawn_program(&pid, "/bin/ls")) {
    return (false);
}

// gets the task
if (!process_get_task(pid, &task)) {
    return (false);
}

// finds the ARM64 image within the tasks address space
if (!get_image_address_by_cputype(
        task,
        &iaddr,
        CPU_TYPE_ARM64
    )) {
    return (false);
}

// dumps 512 bytes from the start of the image
if (!memory_dump(task, iaddr, 512)) {
    return (false);
}
```
