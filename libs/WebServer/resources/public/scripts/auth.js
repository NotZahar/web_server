let email = document.getElementById('email')
let password = document.getElementById('pass')
let authBtn = document.getElementById('auth-btn')

authBtn.onclick = () => {
    fetch('/todos', {
        method: 'POST',
        body: {
            email: email.value,
            password: password.value
        },
        headers: {
            "Content-type": "application/json; charset=UTF-8"
        }
    });
}