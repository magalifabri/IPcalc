#include <stdio.h> // printf
#include <math.h> // pow()
#include <locale.h> // setlocale()

int ft_atoi(const char *str);
char **ft_split(char const *s, char c);

int main(int ac, char **av)
{
    // SETLOCALE is required to print the number of usable hosts with commas between thousands 
    setlocale(LC_NUMERIC, "");

    // CHECK if ac indicates the correct amount of arguments (2)
    if (ac != 2)
    {
        printf("error: incorrect number of arguments\n");
        return (0);
    }
    
    // CHECK length of input to see if it's a full IP address or just the CIDR
    int len = 0;
    while (av[1][len])
        len++;

    // JUST CIDR if len is 2 or lower
    if (len <= 2)
    {
        int CIDR = ft_atoi(av[1]);
        printf("CIDR: %d\n", CIDR);
        // CALCULATE the number of hosts that can connect to the network: 2 ^ ((32 - CIDR) - 2)
        printf("usable hosts: %'.0f\n", pow(2, (32 - CIDR)) - 2);
        return (0);
    }

    // FULL IP address
    // PARSE the argument into the individual bytes and CIDR
    // FIRST split the input string (contained in av[1]) with '.' as delimiter. This returns four four strings, the last one also containing the CIDR.
    char **bytes = ft_split(av[1], '.');
    // SECONDLY split the fourth returned string with '/' as delimiter to separate the last byte and the CIDR. **byte4AndCIDR will then contain the fourth byte at byte4AndCIDR[0] and the CIDR at byte4AndCIDR[1]
    char **byte4AndCIDR = ft_split(bytes[3], '/');

    // USE atoi to convert the bytes and CIDR strings to ints.
    int byte[4];
    byte[0] = ft_atoi(bytes[0]);
    byte[1] = ft_atoi(bytes[1]);
    byte[2] = ft_atoi(bytes[2]);
    byte[3] = ft_atoi(byte4AndCIDR[0]);
    printf("IP address: %d.%d.%d.%d\n", byte[0], byte[1], byte[2], byte[3]);

    int CIDR = ft_atoi(byte4AndCIDR[1]);
    printf("CIDR: %d\n", CIDR);
    
    // PRINT number of usable hosts
    printf("number of usable hosts: %'.0f\n", pow(2, (32 - CIDR)) - 2);

    // CREATE subnet mask
    // To create the subnet mask we simulate turning on a number of bits that is equal to the CIDR, starting from the left. E.g. if the CIDR is 26, then the first three octets (groups of eight bits) are turned on and the 2 most significant bits of the fourth octet. This results in the first three octets being 255 as an int (because 128+64+32+16+8+4+2+1 = 255) and the fourth, 192 (128+64 = 192). The actual calculation for the fourth octet works by subtracting the value of the, in this case 6, bits that the CIDR doesn't contain. As in: 255 - 1+2+4+8+16+32 = 192, or practically 256 - 2^(8 - CIDR). 256 is subtracted from instead of 255, because what is subtracted from it is 1 higher than it should be, because of the 2^n calculation.
    int subnetMask[4];
    int i = 0;
    while (i < 4)
    {
        if (CIDR >= 8)
            subnetMask[i] = 255;
        else if (CIDR >= 0)
            subnetMask[i] = 255 - (pow(2, (8 - CIDR)) - 1);
        else
            subnetMask[i] = 0;
        i++;
        CIDR -= 8;
    }
    printf("subnetmask: %d.%d.%d.%d\n", subnetMask[0], subnetMask[1], subnetMask[2], subnetMask[3]);

    // CREATE host mask
    // We need the host mask to calculate the valid range of host IP addresses. To create the host mask we simply invert the subnet mask with the XOR (^) bitwise operation and 255 as the second operand. This operation compares each bit and returns 1 if they are different. Causing 1s in the subnet mask to be turned into 0s and 0s into 1s. 
    int hostMask[4];
    i = -1;
    while (++i < 4)
        hostMask[i] = subnetMask[i] ^ 255;
    printf("host mask: %d.%d.%d.%d\n", hostMask[0], hostMask[1], hostMask[2], hostMask[3]);

    // CALCULATE the low end of the usable host IP range 
    // For this, turning OFF all host mask bits in the IP address. This is done by taking the IP address and the subnet mask, and using &. & compares two bitfields and returns 1 if both are 1. Because the subnet mask doesn't have any 1s for the host part of the IP address, those are essentially filtered out. Lastly, the last bit needs to be turned on, because otherwise that IP address refers to the network address.
    int IPaddress[4];
    i = -1;
    while (++i < 4)
        IPaddress[i] = byte[i] & subnetMask[i];
    printf("network address: %d.%d.%d.%d\n", IPaddress[0], IPaddress[1], IPaddress[2], IPaddress[3]);
    printf("usable host range min: %d.%d.%d.%d\n", IPaddress[0], IPaddress[1], IPaddress[2], IPaddress[3] + 1);
    
    // CALCULATE the high end of the usable host IP range 
    // For this, turning ON all the host mask bits in the IP address. This is done by taking the IP address and the host mask and using |. | compared two bitfields and returns 1 if either is 1. So it returns a bitfield with both the 1s from the initial IP address as well as all the 1s that are in the host mask. Lastly, the last bit needs to be turned off, because otherwise that IP address refers to the broadcast address.
    i = -1;
    while (++i < 4)
        IPaddress[i] = byte[i] | hostMask[i];
    printf("usable host range max: %d.%d.%d.%d\n", IPaddress[0], IPaddress[1], IPaddress[2], IPaddress[3] - 1);
    printf("broadcast address: %d.%d.%d.%d\n", IPaddress[0], IPaddress[1], IPaddress[2], IPaddress[3]);
}

// https://www.techrepublic.com/article/getting-to-know-your-ip-addresses/
// https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
