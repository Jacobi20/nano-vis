//
//	HASH2.H
//
//	descr : new version of hash-table
//	warning : dependent on 'EStr' 
//


#define HASH_EMPTY		(0)		
#define HASH_DELETED	(void*)(-1)		
#define HASH_NOT_EXISTS	(0)


template <class type, int size> class THash	
	{
		struct	hash_entry_s {
			EStr	key;
			type	value;
		};

	private:
		hash_entry_s	*table[size];

	public:
			THash	( void ) {
				if ( size & (size-1) )
					FATAL("THash<>THash() : size of table must be power of two");
			}

			~THash	( void ) {	DeleteAll(); }

		
		// Returns non-zero hash-value.
	static uint	Hash( const char *key, int n ) 
				{
					uint h = ( RSHash(key)+n )&(size-1);
					if (h==0) h++;
					return h;
				}

		bool	IsEmpty		(uint h)	{ return ( table[ h & (size-1) ]==HASH_EMPTY ); }
		bool	IsDeleted	(uint h)	{ return ( table[ h & (size-1) ]==HASH_DELETED ); }
		bool	IsOccupied	(uint h)	{ return ( !IsEmpty(h) && !IsDeleted(h) ); }

		// Inserts new item to hash table.
		// Returns ZERO if table overflows,
		// otherwise returns hash value of new item
		// Item with existing name will be added to new place.
		//
		// ��������� ����� ������� � ���-�������.
		// � ������ ������������ ������� - ������������ �������� - 0
		// � ��������� ������� - ��� �������� ��������
		// ���� ��� ���������� ������� � ����� �� ������, ����� ������� �����
		// ������� � ������ �����
		uint	Insert	( const char *key, type &value ) 
				{
					uint h;
					for (int i=0; i<size; i++)
					{
						h = Hash(key, i);
						if ( table[h]==HASH_EMPTY || table[h]==HASH_DELETED )
						{
							table[h] = new hash_entry_s;
							table[h]->key	= key;
							table[h]->value	= value;
							return h;
						}
					}
					return 0;
				}
		
		// Searchs for entry with specified name.
		// Returns hash-value of this entry or NULL.
		//
		//	���� ������� � �������� ������
		//	���������� ���-�������� ������������� ��������
		//	��� 0, ���� ������� � ���� ������ �� �����������
		uint	Search	( const char *key ) 
				{
					uint h;
					
					for (int i=0; i<size; i++) 
					{
						h = Hash(key, i);
						if (table[h]==HASH_EMPTY)	break;
						if (table[h]==HASH_DELETED) continue;
						if (strcmpi(table[h]->key.c_str(), key)==0) return h;
					}
					return 0;	
				}

		// Deletes item.
		// If item not found returns false.
		//
		//	������� ������� � ��������� ������
		//	���������� true � ������ ����� � false 
		//	���� ������� �� ����������
		bool	Delete	( const char *key ) 
				{
					uint h = Search(key);
					if (!h) return false;

					delete table[h];
					table[h] = (hash_entry_s*)HASH_DELETED;

					return true;
				}

		// Deletes all items from hash-table.
		//	������� ��� �������� �������
		void	DeleteAll	( void ) 
				{
					for (int i=0; i<size; i++) 
						if (table[i]!=HASH_EMPTY && table[i]!=HASH_DELETED)	{
							delete table[i];
							table[i] = HASH_EMPTY; 
						}
				}

		// Returns pointer to item.
		// Item can be edited.	
		//
		//	���������� ��������� �� ������� � ��������� ���-���������
		//	0 - � ������ ���� ���� ��������� �� �� ������������ �������
		//	��� ��������� �������...
		type	*GetItem	( uint h )
				{
					h = h & (size-1);
					if (table[h]==HASH_EMPTY)	return NULL;
					if (table[h]==HASH_DELETED)	return NULL;
					return &(table[h]->value);
				}

		// Returns name of item.
		//	���������� ��� �������, �� ������� ��������� ���-��������
		const char	*GetName	( uint h )
				{
					h = h & (size-1);
					if (table[h]==HASH_EMPTY)	return NULL;
					if (table[h]==HASH_DELETED)	return NULL;
					return table[h]->key.c_str();
				}
	};
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	