int fib(n)
        unsigned n;
{
        int a = 0, b = 1, tmp; 
        while (n--) {
                tmp = b;
                b += a;
                a = tmp;
        }
        return(b);
}

int main()
{
        return(fib(10));
}
