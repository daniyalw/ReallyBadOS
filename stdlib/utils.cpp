// get the position of the highest value in a list of ints
int get_highest_position(int *arr, int size)
{
    int high_z = 0;
    int high = 0;

    for (int z = 0; z < size; z++)
    {
        if (arr[z] > high)
        {
            high_z = z;
            high = arr[z];
        }
    }

    return high_z;
}

// get the highest value
int get_highest_value(int *arr, int size)
{
    int high = 0;

    for (int z = 0; z < size; z++)
    {
        if (arr[z] > high)
            high = arr[z];
    }

    return high;
}
