// Firebase Configuration and Initialization
import { initializeApp } from 'firebase/app';
import { getFirestore, collection, addDoc, getDocs, deleteDoc, updateDoc, getDoc, doc, query, where, orderBy, writeBatch } from 'firebase/firestore';
import { getAuth, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, onAuthStateChanged } from 'firebase/auth';

// Firebase configuration
const firebaseConfig = {
    apiKey: "AIzaSyBuzgyigHrWYMefmWi_PIobuwJaIHrRH7I",
    authDomain: "networkingbyzuzia.firebaseapp.com",
    projectId: "networkingbyzuzia",
    storageBucket: "networkingbyzuzia.firebasestorage.app",
    messagingSenderId: "942019860902",
    appId: "1:942019860902:web:2d2e01878204fe43c61767",
    measurementId: "G-2ZKM3CW08L"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getFirestore(app);
const auth = getAuth(app);

// Collection name
const CONTACTS_COLLECTION = 'contacts';

function requireUserId() {
    const user = auth.currentUser;
    if (!user) {
        throw new Error('User not authenticated');
    }
    return user.uid;
}

/**
 * Get all contacts from Firestore
 * @returns {Promise<Array>} Array of contact objects
 */
export async function getAllContacts() {
    const userId = requireUserId();
    const contactsCol = collection(db, CONTACTS_COLLECTION);
    const q = query(contactsCol, where('ownerId', '==', userId));
    const snapshot = await getDocs(q);
    const contacts = snapshot.docs.map(doc => ({ docId: doc.id, ...doc.data() }));
    // Sort by ID on client side to avoid needing a composite index
    return contacts.sort((a, b) => a.id - b.id);
}

/**
 * Add new contact to Firestore
 * @param {Object} contact - Contact object
 * @returns {Promise<Object>} Added contact with ID
 */
export async function addContact(contact) {
    const userId = requireUserId();
    const contactsCol = collection(db, CONTACTS_COLLECTION);

    // Get next ID - query without ordering to avoid index requirement
    const q = query(contactsCol, where('ownerId', '==', userId));
    const snapshot = await getDocs(q);
    const allContacts = snapshot.docs.map(doc => doc.data());

    const nextId = allContacts.length > 0
        ? Math.max(...allContacts.map(c => c.id)) + 1
        : 1;

    const contactWithId = { ...contact, id: nextId, ownerId: userId };
    const docRef = await addDoc(contactsCol, contactWithId);

    return { docId: docRef.id, ...contactWithId };
}

/**
 * Delete contact by Firestore document ID
 * @param {string} docId - Firestore document ID
 * @returns {Promise<void>}
 */
export async function deleteContact(docId) {
    const userId = requireUserId();
    const contactDoc = doc(db, CONTACTS_COLLECTION, docId);
    const snapshot = await getDoc(contactDoc);

    if (!snapshot.exists()) {
        throw new Error('Contact not found');
    }

    const data = snapshot.data();
    if (data.ownerId !== userId) {
        throw new Error('You do not have permission to delete this contact.');
    }

    await reassignIntroductions(data, userId);
    await deleteDoc(contactDoc);
}

async function reassignIntroductions(deletedContact, userId) {
    const introducerName = `${deletedContact.name || ''} ${deletedContact.surname || ''}`.trim();
    if (!introducerName) {
        return;
    }

    const contactsCol = collection(db, CONTACTS_COLLECTION);
    const dependentsQuery = query(
        contactsCol,
        where('ownerId', '==', userId),
        where('source_type', '==', 'contact'),
        where('source_value', '==', introducerName)
    );
    const snapshot = await getDocs(dependentsQuery);
    if (snapshot.empty) {
        return;
    }

    const batch = writeBatch(db);
    snapshot.forEach(docSnap => {
        const updates = (deletedContact.source_type === 'event' && deletedContact.source_value)
            ? { source_type: 'event', source_value: deletedContact.source_value }
            : { source_type: '', source_value: '' };
        batch.update(docSnap.ref, updates);
    });
    await batch.commit();
}

/**
 * Update contact by Firestore document ID
 * @param {string} docId - Firestore document ID
 * @param {Object} updates - Partial contact data to update
 * @returns {Promise<void>}
 */
export async function updateContact(docId, updates) {
    const userId = requireUserId();
    const contactDoc = doc(db, CONTACTS_COLLECTION, docId);
    const snapshot = await getDoc(contactDoc);

    if (!snapshot.exists()) {
        throw new Error('Contact not found');
    }

    const data = snapshot.data();
    if (data.ownerId !== userId) {
        throw new Error('You do not have permission to edit this contact.');
    }

    await updateDoc(contactDoc, updates);
}

/**
 * Find contacts by ID
 * @param {number} id - Contact ID
 * @returns {Promise<Array>} Array of matching contacts
 */
export async function findContactById(id) {
    const userId = requireUserId();
    const contactsCol = collection(db, CONTACTS_COLLECTION);
    const q = query(contactsCol, where('ownerId', '==', userId), where('id', '==', id));
    const snapshot = await getDocs(q);
    return snapshot.docs.map(doc => ({ docId: doc.id, ...doc.data() }));
}

/**
 * Find contacts by name and surname
 * @param {string} name - Contact name
 * @param {string} surname - Contact surname
 * @returns {Promise<Array>} Array of matching contacts
 */
export async function findContactByName(name, surname) {
    const userId = requireUserId();
    const contactsCol = collection(db, CONTACTS_COLLECTION);
    const q = query(
        contactsCol,
        where('ownerId', '==', userId),
        where('name', '==', name),
        where('surname', '==', surname)
    );
    const snapshot = await getDocs(q);
    return snapshot.docs.map(doc => ({ docId: doc.id, ...doc.data() }));
}

/**
 * Find contacts by hashtag
 * @param {string} hashtag - Hashtag to search for
 * @returns {Promise<Array>} Array of matching contacts
 */
export async function findContactsByHashtag(hashtag) {
    const userId = requireUserId();
    const contactsCol = collection(db, CONTACTS_COLLECTION);
    const q = query(
        contactsCol,
        where('ownerId', '==', userId),
        where('hashtags', 'array-contains', hashtag.toLowerCase())
    );
    const snapshot = await getDocs(q);
    return snapshot.docs.map(doc => ({ docId: doc.id, ...doc.data() }));
}

/**
 * Find contacts by event (source_value when source_type is 'event')
 * @param {string} event - Event name to search for
 * @returns {Promise<Array>} Array of matching contacts
 */
export async function findContactsByEvent(event) {
    const allContacts = await getAllContacts();
    return allContacts.filter(contact =>
        contact.source_type === 'event' &&
        contact.source_value.toLowerCase().includes(event.toLowerCase())
    );
}

export { db };
export { auth, createUserWithEmailAndPassword, signInWithEmailAndPassword, signOut, onAuthStateChanged };
