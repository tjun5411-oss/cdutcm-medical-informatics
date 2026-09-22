#include <iostream>
#include <string>

using namespace std;
int main()
{
   string raptor_prompt_variable_zzyz;
   ?? i;
   ?? s;
   ?? n;
   ?? m;
   ?? t;

   s =0;
   i =1;
   raptor_prompt_variable_zzyz ="m=";
   cout << raptor_prompt_variable_zzyz << endl;
   cin >> m;
   raptor_prompt_variable_zzyz ="n=";
   cout << raptor_prompt_variable_zzyz << endl;
   cin >> n;
   if (m>n)
   {
      t =m;
      m =n;
      n =t;
   }
   else
   {
   }
   i =m;
   while (!(i>n))
   {
      s =s+i;
      i =i+1;
   }
   cout << s << endl;
   return 0;
}
