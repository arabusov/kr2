int main(void)
{
	char s[] = "this is an example string\n";
	char t[] = "this is an example string \012";
	return strcmp(s, t);
}
