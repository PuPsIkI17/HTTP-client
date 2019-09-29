 Pislari Vadim 323CB Tema 3 PC

	Pentru implementarea temei s-a folosit scheletul de laborator de http, si functiile 
de parsare din "parson". Au fost realizate toate exercitiile cu succes.
	1.Primul exercitiu consta in stabilirea socketului si a conexiunii intre server si 
client si transmiterea unei cereri http "get". 
	2.In al doilea task, in primul rand se determina JSON-un din mesajul primit. Se 
selecteaza cook-ies,  se parcurge mesajul si in momentul in care se identifica un substring
de lungime 11 "Set-Cookie:", se selecteaza ceea ce urmeaza ca fiind cookie. Cu ajutorul 
json_parse_string() se creaza un obiect de tip JSON_Value din string. Se apeleaza functia
json_value_get_object() pentru a obtiue un obiect de tipul JSON_Object. Pentru a accesa un
obiect de tipul JSON_Object cu un anumit nume dintr-un obiect de tipul JSON_Object se 
foloseste json_object_get_object(). Accesand campul data cu ajutorul functiei 
json_object_get_count() putem afla numarul de obiecte. Pentru a forma stringul de forma
username=xxx&password=yyy se parcurge obiectul de la pozitia 0 (cu ajutorul functiei 
json_object_get_value_at()) se identifica numele dupa care si valoare. Cu ajutorul functiilor
strcpy si strcat se formeaza stringul in forma necesara. Acelasi principiu de parsare se 
foloseste si la celalte exercitii. Se stabileste conexiunea si se transmite o cerere http.
Si tipul metodei(GET sau Post), si url-ul si type-ul este selectat de la JSON-ul primit 
de la exercitiul 1 si este apelata la toate subpunctele aceeasi functie compute_request care
in functie de informatia primita ca parametri realizeaza cererea necesara.
	3&4. Parsarea si identificarea parametrilor necesari se face dupa acelasi principiu ca
la exercitiu anterior. Diferenta e ca se face autorizare. Pentru aceasta in functia 
compute_request se adauga campul ""Authorization: Bearer %s"" cu jwt-ul selectat din JSON-ul
de la exercitiul anterior.
	5. Pentru acest exercitiu intii se face o cerere catre api-ul primit ca parametru si se
identifica JSON-ul. Pentru aceasta mai intai se afla adresa IP a url-ului primit ca parametru
cu ajutorul functie gethostbyname() si accesare campului h_addr_list din structura in_addr.
Dupa care se face o cerere catre server cu JSON-ul identificat.

Am observat ca dureaza mai mult timp pentru a primi raspuns de la url-ul trimis ca parametru 
de la subpunctul 5. Mesajul final afisat este "Felicitari, ai obtinut comoara! Ai nota 10!"
Nu am sters afisarile a mesajelor primite in urma cererilor pentru a putea fi observat realizarea
succesiva a exercitiilor in cadrul rularii. 
