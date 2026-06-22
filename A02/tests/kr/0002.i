void memcpy(d, s, sz)
        void *d, *s;
        unsigned sz;
{
        while (*(char *)d++ = *(char *)s++, --sz);
}

int src[] = { 1, 2, 3 };
main()
{
        char d[3];
        memcpy(d, src, 3*sizeof(src[0]));
        return(0);
}
