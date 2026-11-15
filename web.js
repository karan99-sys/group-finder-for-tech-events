
const groups = [
  {id:1,name:"Code Masters",category:"DSA & Competitive Coding",type:"Web Development",description:"A team focused on problem-solving, coding contests and useful tools.",skills:["C++","DSA","Git"],members:3,capacity:5,lead:"Aarav Sharma",initials:"CM",color:"violet"},
  {id:2,name:"AI Warriors",category:"Artificial Intelligence",type:"AI/ML",description:"Exploring machine learning and AI-powered projects.",skills:["Python","ML","Data"],members:3,capacity:5,lead:"Riya Verma",initials:"AW",color:"blue"},
  {id:3,name:"Web Wizards",category:"Web Development",type:"Web Development",description:"Building responsive websites and web applications.",skills:["HTML","CSS","JavaScript"],members:4,capacity:6,lead:"Kabir Singh",initials:"WW",color:"orange"},
  {id:4,name:"Pixel Pioneers",category:"UI / UX Design",type:"Design",description:"Creating intuitive interfaces and prototypes.",skills:["Figma","UI/UX","CSS"],members:2,capacity:4,lead:"Meera Joshi",initials:"PP",color:"pink"},
  {id:5,name:"App Alchemists",category:"Mobile Applications",type:"App Development",description:"Building helpful mobile applications.",skills:["Java","Android","Firebase"],members:3,capacity:5,lead:"Dev Patel",initials:"AA",color:"green"},
  {id:6,name:"ML Mavericks",category:"Machine Learning",type:"AI/ML",description:"Experimenting with datasets and ML models.",skills:["Python","Pandas","Scikit-learn"],members:2,capacity:5,lead:"Ananya Das",initials:"MM",color:"blue"}
];

const students = [
  {name:"Aaditya Kandari",id:"TN101",skills:"C++, DSA, HTML",initials:"AK"},
  {name:"Riya Verma",id:"TN102",skills:"Python, Machine Learning",initials:"RV"},
  {name:"Kabir Singh",id:"TN103",skills:"HTML, CSS, JavaScript",initials:"KS"},
  {name:"Meera Joshi",id:"TN104",skills:"Figma, UI/UX Design",initials:"MJ"},
  {name:"Dev Patel",id:"TN105",skills:"Java, Android",initials:"DP"}
];

let activeFilter = "All";
const STORAGE_KEY = "teamNexusRequests";

let requests = [];
try {
  requests = JSON.parse(localStorage.getItem(STORAGE_KEY) || "[]");
  if (!Array.isArray(requests)) requests = [];
} catch {
  requests = [];
}

const groupGrid = document.getElementById("groupGrid");
const studentList = document.getElementById("studentList");
const requestList = document.getElementById("requestList");
const searchInput = document.getElementById("searchInput");

function saveRequests() {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(requests));
}

function showToast(message) {
  const toast = document.getElementById("toast");
  toast.textContent = message;
  toast.classList.add("show");

  clearTimeout(window.toastTimer);
  window.toastTimer = setTimeout(() => {
    toast.classList.remove("show");
  }, 2600);
}

function renderGroups() {
  const query = searchInput.value.toLowerCase().trim();

  const filtered = groups.filter(group => {
    const matchesFilter =
      activeFilter === "All" || group.type === activeFilter;

    const text = [
      group.name,
      group.category,
      group.description,
      group.lead,
      ...group.skills
    ].join(" ").toLowerCase();

    return matchesFilter && text.includes(query);
  });

  if (filtered.length === 0) {
    groupGrid.innerHTML =
      `<div class="no-results">No groups found. Try another search.</div>`;
    return;
  }

  groupGrid.innerHTML = filtered.map(group => {
    const alreadyRequested = requests.some(r => r.id === group.id);
    const full = group.members >= group.capacity;

    return `
      <article class="group-card">
        <div class="group-card-top">
          <div class="group-logo ${group.color}">
            ${group.initials}
          </div>
          <span class="open-label">
            ${full ? "● Full" : "● Looking for members"}
          </span>
        </div>

        <p class="group-category">${group.category}</p>
        <h3>${group.name}</h3>
        <p class="group-description">${group.description}</p>

        <div class="skill-list">
          ${group.skills.map(skill => `<span>${skill}</span>`).join("")}
        </div>

        <div class="group-meta">
          <span>♙ ${group.members}/${group.capacity} members</span>
          <span>Lead: ${group.lead}</span>
        </div>

        <button class="request-btn"
          data-group-id="${group.id}"
          ${full || alreadyRequested ? "disabled" : ""}>
          ${full ? "Group is full" : alreadyRequested ? "Request sent" : "Request to join"}
          <span>→</span>
        </button>
      </article>
    `;
  }).join("");
}

function renderStudents() {
  studentList.innerHTML = students.map(student => `
    <div class="student-row">
      <span class="avatar student-avatar">${student.initials}</span>
      <div class="student-info">
        <strong>${student.name}</strong>
        <small>${student.id} · ${student.skills}</small>
      </div>
      <span class="online-dot"></span>
    </div>
  `).join("");

  document.getElementById("studentCount").textContent = "36";
  document.getElementById("groupCount").textContent = "15";
}

function sendRequest(id) {
  const group = groups.find(g => g.id === id);

  if (!group) return;

  if (group.members >= group.capacity) {
    showToast("This group is full.");
    return;
  }

  if (requests.some(r => r.id === id)) {
    showToast("You already requested to join this group.");
    return;
  }

  requests.push({
    id: group.id,
    name: group.name,
    lead: group.lead,
    status: "Pending"
  });

  saveRequests();
  renderGroups();
  renderRequests();

  showToast("Request sent to " + group.name);
}

function cancelRequest(id) {
  requests = requests.filter(r => r.id !== id);

  saveRequests();
  renderGroups();
  renderRequests();

  showToast("Join request cancelled.");
}

function renderRequests() {
  const count = requests.length;

  document.getElementById("requestCount").textContent = count;
  document.getElementById("requestBadge").textContent =
    count + " pending";

  if (count === 0) {
    requestList.innerHTML = `
      <div class="empty-state">
        <span>✉</span>
        <p>No requests yet</p>
        <small>Send a request to a group to see it here.</small>
      </div>`;
    return;
  }

  requestList.innerHTML = requests.map(request => `
    <div class="request-row">
      <span class="request-icon">↗</span>
      <div>
        <strong>${request.name}</strong>
        <small>Request sent to ${request.lead}</small>
      </div>
      <span class="pending">${request.status}</span>
      <button class="cancel-request"
        data-cancel-id="${request.id}"
        aria-label="Cancel request">×</button>
    </div>
  `).join("");
}

// Search functionality
searchInput.addEventListener("input", renderGroups);

// Category filters
document.querySelectorAll(".filter").forEach(button => {
  button.addEventListener("click", () => {
    document.querySelector(".filter.active")?.classList.remove("active");

    button.classList.add("active");
    activeFilter = button.dataset.filter;

    renderGroups();
  });
});

// Send join request
groupGrid.addEventListener("click", event => {
  const button = event.target.closest("[data-group-id]");

  if (button && !button.disabled) {
    sendRequest(Number(button.dataset.groupId));
  }
});

// Cancel join request
requestList.addEventListener("click", event => {
  const button = event.target.closest("[data-cancel-id]");

  if (button) {
    cancelRequest(Number(button.dataset.cancelId));
  }
});

// Initial display
renderGroups();
renderStudents();
renderRequests();
