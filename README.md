Echipa Tema 3 SD
================

### Autori:
- **Maria Diana Vulpoi**
- **Vlad Mihail David**

---

## **Friends.c**

### **Add_friends**
Creează o legătură de prietenie între doi utilizatori, adăugând o muchie între cele două noduri din graf care îi reprezintă.

### **Remove_friends**
Șterge legătura de prietenie dintre doi utilizatori, eliminând muchia dintre cele două noduri din graf care îi reprezintă.

### **Find_distance**
Folosește BFS prin apelarea funcției `graph_distance` pentru a găsi distanța dintre doi utilizatori de pe platformă.

### **Neighbours_of_neighbours**
Sugerează prieteni pentru un utilizator dat. Obține ID-ul utilizatorului și creează o listă pentru sugestii.
- Parcurge vecinii utilizatorului și, pentru fiecare vecin, parcurge vecinii acestora, adăugând în lista de sugestii utilizatorii care nu sunt deja prieteni direcți.
- După eliminarea duplicatelor, afișează sugestiile și exclude utilizatorul curent dacă apare în listă.
- Eliberează memoria alocată la final.

### **Print_commons**
Afișează prietenii comuni ai doi utilizatori.
- Obține ID-urile celor doi utilizatori și creează o listă pentru prietenii comuni.
- Parcurge listele de prieteni și identifică prietenii comuni.
- Dacă nu există prieteni comuni, afișează un mesaj corespunzător și eliberează memoria.
- În caz contrar, afișează prietenii comuni și eliberează memoria alocată.

### **Count_friends**
Primește un utilizator și afișează numărul de prieteni ai acestuia, determinat de dimensiunea listei de prieteni.

### **Popular_friend**
Identifică cel mai popular prieten al utilizatorului.
- Parcurge lista de prieteni și determină numărul de prieteni ai fiecăruia.
- Dacă un prieten are mai mulți prieteni decât valoarea curentă de popularitate, actualizează ID-ul și valoarea popularității.
- Dacă există egalitate, se alege prietenul cu ID-ul mai mic.

### **Handle_popular**
Apelează `popular_friend` și afișează un mesaj corespunzător în funcție de rezultat.

---

## **Posts.c**

Programul utilizează o structură de date bazată pe liste care formează un arbore:
- Fiecare postare (`post_t`) poate avea o listă de repostări.
- Fiecare repostare poate avea, la rândul său, propria listă de repostări.

### **Create_post**
Creează o nouă postare într-o listă simplu înlănțuită, incrementând contorul `next_post_id`.
- După creare, adaugă un nou nod și afișează un mesaj corespunzător (postare nouă sau repostare).

### **Repost**
Creează un repost al unei postări existente.
- Parcurge lista de postări și creează repostul când ajunge la postarea cu ID-ul indicat.

### **Print_posts**
Afișează toate repostările.

### **Get_reposts**
Parcurge recursiv arborele de postări și afișează întreaga ierarhie pentru o postare sau un repost.

### **Like**
Adaugă sau elimină un like/dislike la o postare sau repostare.
- Dacă postarea nu a primit like anterior, adaugă nodul în lista `likes` și afișează un mesaj corespunzător.
- Dacă a primit deja like, elimină nodul și afișează un mesaj pentru dislike.

### **Get_likes**
Afișează numărul de aprecieri ale unei postări, determinat de lungimea listei `likes`.

### **Check_max**
Verifică dacă o postare are mai multe aprecieri decât un anumit prag.
- Dacă da, actualizează atât valoarea pragului, cât și identificatorul asociat acestuia.

### **Ratio**
Verifică dacă o repostare are mai multe aprecieri decât postarea originală, utilizând funcția `check_max`.

### **Delete**
Șterge recursiv o postare și toate repostările acesteia.

### **Lca2**
Determină cel mai mic repost comun a două postări.
- Dacă ID-ul postării curente este egal cu unul dintre cele două ID-uri căutate, acesta este cel mai mic repost comun.
- Se creează un vector `found_ids` pentru a salva ID-ul repostului comun.
- Se parcurg repostările și se salvează în `found_ids` ID-urile strămoșului găsit.
- Dacă ambele valori sunt diferite de `-1`, postarea curentă este cel mai mic strămoș comun.

### **Lca3**
Găsește cel mai mic repost comun a două postări utilizând `lca2`.
- Dacă `lca2` returnează un ID valid, acesta este strămoșul comun.
- Dacă returnează `-1`, afișează un mesaj indicând că nu există un strămoș comun.

### **Free_post**
Eliberează toată memoria utilizată în program.

---

## **Feed.c**

### **Feed**
Afișează postările recente pe baza relațiilor de prietenie.
- Parcurge postările și verifică dacă utilizatorul care a postat este prieten cu utilizatorul dat sau este chiar el însuși.
- Dacă da, afișează postarea și decrementează `feed_size`, care controlează numărul maxim de postări afișate.

### **View_profile**
Găsește și afișează toate postările și repostările unui utilizator.

### **Friends_repost**
Afișează toți prietenii unui utilizator care au dat repost la o anumită postare.