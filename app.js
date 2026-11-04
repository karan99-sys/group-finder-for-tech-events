
class Student {
  constructor(id, name, skill1, skill2) {
    this.id = Number(id);
    this.name = name;
    this.skill1 = skill1;
    this.skill2 = skill2;
  }
}

class MemberNode {
  constructor(student) {
    this.student = student;
    this.next = null;
  }
}

class Group {
  constructor(id, name, skill1, skill2) {
    this.groupID = Number(id);
    this.groupName = name;
    this.requiredSkill1 = skill1;
    this.requiredSkill2 = skill2;
    this.head = null; // Linked List head
  }

  compatibility(student) {
    let score = 0;
    if (student.skill1 === this.requiredSkill1 || student.skill2 === this.requiredSkill1) {
      score += 50;
    }
    if (student.skill1 === this.requiredSkill2 || student.skill2 === this.requiredSkill2) {
      score += 50;
    }
    return score;
  }

  addMember(student) {
    const newNode = new MemberNode(student);
    if (this.head === null) {
      this.head = newNode;
    } else {
      let temp = this.head;
      while (temp.next !== null) {
        temp = temp.next;
      }
      temp.next = newNode;
    }
  }

  getMembers() {
    const members = [];
    let temp = this.head;
    while (temp !== null) {
      members.push(temp.student);
      temp = temp.next;
    }
    return members;
  }
}

class JoinRequest {
  constructor(student, groupID) {
    this.student = student;
    this.groupID = Number(groupID);
    this.timestamp = new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  }
}

class RequestQueue {
  constructor(capacity = 50) {
    this.arr = new Array(capacity);
    this.front = 0;
    this.rear = -1;
    this.capacity = capacity;
  }

  isEmpty() {
    return this.front > this.rear;
  }

  isFull() {
    return this.rear === this.capacity - 1;
  }

  enqueue(request) {
    if (this.isFull()) {
      return false;
    }
    this.rear++;
    this.arr[this.rear] = request;
    return true;
  }

  dequeue() {
    if (this.isEmpty()) {
      return null;
    }
    const request = this.arr[this.front];
    this.front++;
    return request;
  }

  peek() {
    if (this.isEmpty()) return null;
    return this.arr[this.front];
  }

  size() {
    if (this.isEmpty()) return 0;
    return (this.rear - this.front + 1);
  }

  getAll() {
    if (this.isEmpty()) return [];
    const list = [];
    for (let i = this.front; i <= this.rear; i++) {
      list.push({
        index: i,
        isFront: i === this.front,
        isRear: i === this.rear,
        request: this.arr[i]
      });
    }
    return list;
  }
}

// ----------------------------------------------------------------------------
// 2. STATE INITIALIZATION & PERSISTENCE
// ----------------------------------------------------------------------------

const STORAGE_KEY = "campus_pbl_state_v1";

let students = [];
let groups = [];
let requestsQueue = new RequestQueue(50);
let currentDequeuedRequest = null;

function loadDefaultState() {
  students = [
    new Student(101, "Aaditya", "C++", "DSA"),
    new Student(102, "Karan", "Python", "AI"),
    new Student(103, "Amit", "HTML", "CSS")
  ];

  groups = [
    new Group(1, "Code Masters", "C++", "DSA"),
    new Group(2, "AI Warriors", "Python", "AI"),
    new Group(3, "Web Wizards", "HTML", "CSS")
  ];

  requestsQueue = new RequestQueue(50);
  currentDequeuedRequest = null;
  saveState();
}

function saveState() {
  const plainGroups = groups.map(g => ({
    groupID: g.groupID,
    groupName: g.groupName,
    requiredSkill1: g.requiredSkill1,
    requiredSkill2: g.requiredSkill2,
    members: g.getMembers()
  }));

  const state = {
    students,
    groups: plainGroups,
    queueItems: requestsQueue.getAll().map(item => item.request),
    currentDequeuedRequest
  };

  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(state));
  } catch (e) {
    console.warn("Could not save to localStorage", e);
  }
}

function loadState() {
  const saved = localStorage.getItem(STORAGE_KEY);
  if (!saved) {
    loadDefaultState();
    return;
  }

  try {
    const data = JSON.parse(saved);
    students = (data.students || []).map(s => new Student(s.id, s.name, s.skill1, s.skill2));
    
    groups = (data.groups || []).map(g => {
      const group = new Group(g.groupID, g.groupName, g.requiredSkill1, g.requiredSkill2);
      if (Array.isArray(g.members)) {
        g.members.forEach(m => group.addMember(new Student(m.id, m.name, m.skill1, m.skill2)));
      }
      return group;
    });

    requestsQueue = new RequestQueue(50);
    if (Array.isArray(data.queueItems)) {
      data.queueItems.forEach(item => {
        const student = new Student(item.student.id, item.student.name, item.student.skill1, item.student.skill2);
        requestsQueue.enqueue(new JoinRequest(student, item.groupID));
      });
    }

    currentDequeuedRequest = data.currentDequeuedRequest || null;
  } catch (err) {
    console.error("Failed to parse saved state, reverting to defaults", err);
    loadDefaultState();
  }
}

// ----------------------------------------------------------------------------
// 3. TERMINAL CONSOLE LOGGER
// ----------------------------------------------------------------------------

function logTerminal(text) {
  const terminal = document.getElementById("terminal-output");
  if (!terminal) return;
  const timestamp = new Date().toLocaleTimeString();
  terminal.textContent += `[${timestamp}] ${text}\n`;
  terminal.scrollTop = terminal.scrollHeight;
}

// ----------------------------------------------------------------------------
// 4. TOAST NOTIFICATIONS
// ----------------------------------------------------------------------------

function showToast(message, type = "info") {
  const container = document.getElementById("toast-container");
  if (!container) return;

  const toast = document.createElement("div");
  toast.className = `toast ${type}`;
  const icon = type === "success" ? "✅" : type === "danger" ? "❌" : "ℹ️";
  toast.innerHTML = `<span>${icon}</span> <span>${message}</span>`;

  container.appendChild(toast);
  setTimeout(() => {
    toast.style.opacity = "0";
    setTimeout(() => toast.remove(), 250);
  }, 3200);
}

// ----------------------------------------------------------------------------
// 5. UI RENDERING FUNCTIONS
// ----------------------------------------------------------------------------

function updateStats() {
  document.getElementById("stat-students-count").textContent = students.length;
  document.getElementById("stat-groups-count").textContent = groups.length;
  const qSize = requestsQueue.size();
  document.getElementById("stat-queue-count").textContent = qSize;
  document.getElementById("queue-badge").textContent = qSize;
  
  const totalMembers = groups.reduce((acc, g) => acc + g.getMembers().length, 0);
  document.getElementById("stat-members-count").textContent = totalMembers;
}

function renderGroups() {
  const container = document.getElementById("groups-container");
  container.innerHTML = "";

  groups.forEach(group => {
    const members = group.getMembers();

    let chainHtml = "";
    if (members.length === 0) {
      chainHtml = `<span class="node-null">head -> NULL (No members yet)</span>`;
    } else {
      chainHtml = members.map(m => `
        <div class="member-node">
          <span class="node-icon">👤</span>
          <span>${m.name}</span>
        </div>
        <span class="node-arrow">&rarr;</span>
      `).join("") + `<span class="node-null">NULL</span>`;
    }

    const card = document.createElement("div");
    card.className = "group-card";
    card.innerHTML = `
      <div class="group-header">
        <div class="group-title-box">
          <h3 class="group-name">${group.groupName}</h3>
          <span class="group-id-pill">ID: ${group.groupID}</span>
        </div>
      </div>

      <div class="skills-wrapper">
        <span class="skills-label">Required Skills:</span>
        <span class="skill-tag">${group.requiredSkill1}</span>
        <span class="skill-tag alt">${group.requiredSkill2}</span>
      </div>

      <div class="linked-list-section">
        <div class="linked-list-header">
          <span class="linked-list-title">
            <span>🔗 Linked List Members</span>
          </span>
          <span class="mono-badge">${members.length} node(s)</span>
        </div>
        <div class="linked-list-chain">
          ${chainHtml}
        </div>
      </div>

      <div class="group-actions">
        <button class="btn btn-sm btn-outline" onclick="openJoinModalForGroup(${group.groupID})">
          + Request to Join
        </button>
      </div>
    `;
    container.appendChild(card);
  });
}

function renderStudents() {
  const container = document.getElementById("students-container");
  container.innerHTML = "";

  students.forEach(student => {
    const card = document.createElement("div");
    card.className = "student-card";
    const initial = student.name ? student.name.charAt(0).toUpperCase() : "?";

    card.innerHTML = `
      <div class="student-card-header">
        <div class="avatar-circle">${initial}</div>
        <div>
          <h4 class="student-name">${student.name}</h4>
          <span class="student-id">ID: ${student.id}</span>
        </div>
      </div>
      <div class="skills-wrapper">
        <span class="skills-label">Skills:</span>
        <span class="skill-tag">${student.skill1}</span>
        <span class="skill-tag alt">${student.skill2}</span>
      </div>
      <button class="btn btn-sm btn-outline" style="width: 100%; margin-top: 0.5rem;" onclick="quickMatchStudent(${student.id})">
        🎯 Find Best Group
      </button>
    `;
    container.appendChild(card);
  });

  populateDropdowns();
}

function populateDropdowns() {
  const matchSelect = document.getElementById("match-student-select");
  const reqStudentSelect = document.getElementById("req-student-select");
  const reqGroupSelect = document.getElementById("req-group-select");

  if (matchSelect) {
    matchSelect.innerHTML = students.map(s => 
      `<option value="${s.id}">ID ${s.id} - ${s.name} (${s.skill1}, ${s.skill2})</option>`
    ).join("");
  }

  if (reqStudentSelect) {
    reqStudentSelect.innerHTML = students.map(s => 
      `<option value="${s.id}">ID ${s.id} - ${s.name}</option>`
    ).join("");
  }

  if (reqGroupSelect) {
    reqGroupSelect.innerHTML = groups.map(g => 
      `<option value="${g.groupID}">Group ${g.groupID} - ${g.groupName} (${g.requiredSkill1}, ${g.requiredSkill2})</option>`
    ).join("");
  }
}

function renderQueue() {
  const track = document.getElementById("queue-visual-track");
  const statusText = document.getElementById("queue-status-text");
  const qItems = requestsQueue.getAll();

  statusText.textContent = `Status: ${qItems.length} / ${requestsQueue.capacity} in Queue`;

  if (qItems.length === 0) {
    track.innerHTML = `<div class="queue-empty-state"><div class="empty-icon">📭</div>No requests currently in queue.</div>`;
  } else {
    track.innerHTML = qItems.map(item => {
      const targetGroup = groups.find(g => g.groupID === item.request.groupID);
      const groupName = targetGroup ? targetGroup.groupName : `Group #${item.request.groupID}`;
      
      let badge = "";
      if (item.isFront && item.isRear) {
        badge = `<span class="queue-pointer front">FRONT &amp; REAR [arr[${item.index}]]</span>`;
      } else if (item.isFront) {
        badge = `<span class="queue-pointer front">&rarr; FRONT [arr[${item.index}]]</span>`;
      } else if (item.isRear) {
        badge = `<span class="queue-pointer rear">REAR [arr[${item.index}]] &larr;</span>`;
      } else {
        badge = `<span class="mono-badge">arr[${item.index}]</span>`;
      }

      return `
        <div class="queue-item-card">
          <div>
            <strong>${item.request.student.name}</strong> 
            <span style="color: var(--text-muted); font-size: 0.85rem;">(ID ${item.request.student.id})</span>
            <div style="font-size: 0.8rem; color: var(--info); margin-top: 0.2rem;">
              Requested: <strong>${groupName}</strong>
            </div>
          </div>
          <div>${badge}</div>
        </div>
      `;
    }).join("");
  }

  renderPendingProcessBox();
}

function renderPendingProcessBox() {
  const box = document.getElementById("pending-request-box");

  if (!currentDequeuedRequest) {
    if (requestsQueue.isEmpty()) {
      box.innerHTML = `
        <div class="pending-empty-state">
          <div class="empty-icon">📭</div>
          <p>No pending join requests to process.</p>
          <button class="btn btn-sm btn-outline" id="quick-send-sample-req">Send a Test Request</button>
        </div>
      `;
      const quickBtn = document.getElementById("quick-send-sample-req");
      if (quickBtn) {
        quickBtn.addEventListener("click", () => {
          if (students.length > 0 && groups.length > 0) {
            enqueueJoinRequest(students[0].id, groups[0].groupID);
          }
        });
      }
    } else {
      box.innerHTML = `
        <div style="text-align: center; padding: 2rem 1rem;">
          <p style="margin-bottom: 1.25rem; color: var(--text-muted);">
            There are <strong>${requestsQueue.size()}</strong> request(s) waiting in queue.
          </p>
          <button class="btn btn-primary" id="btn-dequeue-next">
            📥 Dequeue Next Request (Choice 5)
          </button>
        </div>
      `;
      document.getElementById("btn-dequeue-next").addEventListener("click", dequeueNextRequest);
    }
    return;
  }

  // Active dequeued request awaiting accept/reject
  const targetGroup = groups.find(g => g.groupID === currentDequeuedRequest.groupID);
  const groupName = targetGroup ? targetGroup.groupName : `ID ${currentDequeuedRequest.groupID}`;

  box.innerHTML = `
    <div class="process-box-active">
      <div style="font-size: 0.75rem; text-transform: uppercase; color: var(--warning); font-weight: 700; margin-bottom: 0.5rem;">
        ⚠️ Dequeued From Front - Awaiting Review
      </div>
      <div class="req-detail-row">
        <span class="req-label">Student Name:</span>
        <span class="req-val">${currentDequeuedRequest.student.name}</span>
      </div>
      <div class="req-detail-row">
        <span class="req-label">Student ID:</span>
        <span class="req-val">${currentDequeuedRequest.student.id}</span>
      </div>
      <div class="req-detail-row">
        <span class="req-label">Student Skills:</span>
        <span class="req-val">${currentDequeuedRequest.student.skill1}, ${currentDequeuedRequest.student.skill2}</span>
      </div>
      <div class="req-detail-row">
        <span class="req-label">Target Group:</span>
        <span class="req-val">${groupName} (ID: ${currentDequeuedRequest.groupID})</span>
      </div>

      <div class="process-actions">
        <button class="btn btn-success" id="btn-accept-request">
          ✅ Accept (Add to Linked List)
        </button>
        <button class="btn btn-danger" id="btn-reject-request">
          ❌ Reject Request
        </button>
      </div>
    </div>
  `;

  document.getElementById("btn-accept-request").addEventListener("click", () => handleRequestDecision(true));
  document.getElementById("btn-reject-request").addEventListener("click", () => handleRequestDecision(false));
}

// ----------------------------------------------------------------------------
// 6. CORE LOGIC / C++ OPERATIONS
// ----------------------------------------------------------------------------

function dequeueNextRequest() {
  if (requestsQueue.isEmpty()) {
    showToast("No pending requests in queue.", "warning");
    logTerminal("Choice 5: No pending requests.");
    return;
  }

  currentDequeuedRequest = requestsQueue.dequeue();
  logTerminal(`Choice 5: Request dequeued from: ${currentDequeuedRequest.student.name} for Group ID: ${currentDequeuedRequest.groupID}`);
  saveState();
  renderQueue();
  updateStats();
}

function handleRequestDecision(isAccepted) {
  if (!currentDequeuedRequest) return;

  const req = currentDequeuedRequest;
  if (isAccepted) {
    const group = groups.find(g => g.groupID === req.groupID);
    if (group) {
      group.addMember(req.student);
      showToast(`Accepted! ${req.student.name} added to ${group.groupName}'s Linked List.`, "success");
      logTerminal(`Choice 5: Request accepted. Student ${req.student.name} added to group ${group.groupName}.`);
    } else {
      showToast("Group not found.", "danger");
    }
  } else {
    showToast(`Rejected request from ${req.student.name}.`, "danger");
    logTerminal(`Choice 5: Request from ${req.student.name} rejected.`);
  }

  currentDequeuedRequest = null;
  saveState();
  renderGroups();
  renderQueue();
  updateStats();
}

function enqueueJoinRequest(studentID, groupID) {
  const student = students.find(s => s.id === Number(studentID));
  const group = groups.find(g => g.groupID === Number(groupID));

  if (!student) {
    showToast("Student not found.", "danger");
    logTerminal("Choice 4: Student not found.");
    return;
  }
  if (!group) {
    showToast("Group not found.", "danger");
    logTerminal("Choice 4: Group not found.");
    return;
  }

  const request = new JoinRequest(student, group.groupID);
  const success = requestsQueue.enqueue(request);

  if (success) {
    showToast(`Join request for "${group.groupName}" enqueued successfully!`, "success");
    logTerminal(`Choice 4: Join request sent for student ${student.name} to Group ${group.groupName}.`);
    saveState();
    renderQueue();
    updateStats();
  } else {
    showToast("Request queue is full (Max 50).", "danger");
    logTerminal("Choice 4: Request queue is full.");
  }
}

function calculateCompatibility(studentID) {
  const student = students.find(s => s.id === Number(studentID));
  if (!student) {
    showToast("Student not found.", "danger");
    return;
  }

  logTerminal(`\n--- Choice 3: Finding Best Group for ${student.name} (ID: ${student.id}) ---`);
  logTerminal(`Student Skills: ${student.skill1}, ${student.skill2}`);
  logTerminal("Compatibility Scores:");

  let bestScore = -1;
  let bestGroup = null;

  const scoredGroups = groups.map(g => {
    const score = g.compatibility(student);
    logTerminal(`${g.groupName} = ${score}%`);
    if (score > bestScore) {
      bestScore = score;
      bestGroup = g;
    }
    return { group: g, score };
  });

  if (bestGroup) {
    logTerminal(`Best Group: ${bestGroup.groupName} (${bestScore}% match)\n`);
  }

  // Render matchmaker UI
  const wrapper = document.getElementById("match-results-wrapper");
  wrapper.classList.remove("hidden");

  const banner = document.getElementById("best-match-banner");
  if (bestGroup) {
    banner.innerHTML = `
      <div>
        <div class="best-match-badge">🏆 Recommended Best Match</div>
        <div class="best-match-name">${bestGroup.groupName}</div>
        <div style="font-size: 0.85rem; color: var(--text-muted); margin-top: 0.2rem;">
          Matches skills: ${bestGroup.requiredSkill1}, ${bestGroup.requiredSkill2}
        </div>
      </div>
      <div style="display: flex; align-items: center; gap: 1rem;">
        <div class="best-match-score">${bestScore}%</div>
        <button class="btn btn-primary" onclick="enqueueJoinRequest(${student.id}, ${bestGroup.groupID})">
          Send Join Request
        </button>
      </div>
    `;
  }

  const list = document.getElementById("compatibility-list");
  list.innerHTML = scoredGroups.map(item => {
    const fillClass = item.score >= 100 ? "high" : item.score >= 50 ? "mid" : "low";
    return `
      <div class="compat-item">
        <div class="compat-group-info">
          <strong>${item.group.groupName}</strong>
          <span style="color: var(--text-muted); font-size: 0.8rem; margin-left: 0.5rem;">
            (${item.group.requiredSkill1}, ${item.group.requiredSkill2})
          </span>
          <div class="compat-bar-wrap">
            <div class="compat-bar-fill ${fillClass}" style="width: ${item.score}%"></div>
          </div>
        </div>
        <div style="display: flex; align-items: center; gap: 0.75rem;">
          <span style="font-weight: 700; font-size: 1.1rem; color: ${item.score >= 50 ? 'white' : 'var(--text-dim)'};">
            ${item.score}%
          </span>
          <button class="btn btn-sm btn-outline" onclick="enqueueJoinRequest(${student.id}, ${item.group.groupID})">
            Join
          </button>
        </div>
      </div>
    `;
  }).join("");
}

// ----------------------------------------------------------------------------
// 7. EVENT LISTENERS & MODAL HANDLERS
// ----------------------------------------------------------------------------

function openJoinModalForGroup(groupID) {
  const select = document.getElementById("req-group-select");
  if (select) select.value = groupID;
  document.getElementById("modal-send-request").classList.remove("hidden");
}

function quickMatchStudent(studentID) {
  const tabBtn = document.querySelector('[data-tab="tab-matchmaker"]');
  if (tabBtn) tabBtn.click();
  const select = document.getElementById("match-student-select");
  if (select) {
    select.value = studentID;
    calculateCompatibility(studentID);
  }
}

document.addEventListener("DOMContentLoaded", () => {
  loadState();

  // Initial Logs matching main.cpp
  logTerminal("==============================");
  logTerminal("     CAMPUS GROUP FINDER");
  logTerminal("==============================");
  logTerminal("System initialized successfully with C++ DSA models.");
  logTerminal(`Loaded ${students.length} students, ${groups.length} groups.`);

  // Render Views
  renderGroups();
  renderStudents();
  renderQueue();
  updateStats();

  // Tab Switching
  document.querySelectorAll(".tab-btn").forEach(btn => {
    btn.addEventListener("click", () => {
      document.querySelectorAll(".tab-btn").forEach(b => b.classList.remove("active"));
      document.querySelectorAll(".tab-pane").forEach(p => p.classList.remove("active"));

      btn.classList.add("active");
      const target = document.getElementById(btn.dataset.tab);
      if (target) target.classList.add("active");

      // Log menu selection
      const tabName = btn.textContent.trim();
      logTerminal(`Navigated to: ${tabName}`);
    });
  });

  // Matchmaker Calculate Button
  document.getElementById("calculate-match-btn").addEventListener("click", () => {
    const studentID = document.getElementById("match-student-select").value;
    calculateCompatibility(studentID);
  });

  // Add Student Modal Handlers
  const addStudentModal = document.getElementById("modal-add-student");
  document.getElementById("open-add-student-modal").addEventListener("click", () => {
    addStudentModal.classList.remove("hidden");
  });
  document.getElementById("close-student-modal").addEventListener("click", () => {
    addStudentModal.classList.add("hidden");
  });
  document.getElementById("cancel-student-modal").addEventListener("click", () => {
    addStudentModal.classList.add("hidden");
  });

  document.getElementById("form-add-student").addEventListener("submit", (e) => {
    e.preventDefault();
    const id = Number(document.getElementById("student-id").value);
    const name = document.getElementById("student-name").value.trim();
    const s1 = document.getElementById("student-skill1").value.trim();
    const s2 = document.getElementById("student-skill2").value.trim();

    if (students.some(s => s.id === id)) {
      showToast(`Student with ID ${id} already exists!`, "danger");
      return;
    }

    const newStudent = new Student(id, name, s1, s2);
    students.push(newStudent);
    saveState();
    renderStudents();
    updateStats();

    showToast(`Student "${name}" registered successfully!`, "success");
    logTerminal(`Registered new Student: ID: ${id} | Name: ${name} | Skills: ${s1}, ${s2}`);

    e.target.reset();
    addStudentModal.classList.add("hidden");
  });

  // Add Group Modal Handlers
  const addGroupModal = document.getElementById("modal-add-group");
  document.getElementById("open-add-group-modal").addEventListener("click", () => {
    addGroupModal.classList.remove("hidden");
  });
  document.getElementById("close-group-modal").addEventListener("click", () => {
    addGroupModal.classList.add("hidden");
  });
  document.getElementById("cancel-group-modal").addEventListener("click", () => {
    addGroupModal.classList.add("hidden");
  });

  document.getElementById("form-add-group").addEventListener("submit", (e) => {
    e.preventDefault();
    const id = Number(document.getElementById("group-id").value);
    const name = document.getElementById("group-name").value.trim();
    const s1 = document.getElementById("group-skill1").value.trim();
    const s2 = document.getElementById("group-skill2").value.trim();

    if (groups.some(g => g.groupID === id)) {
      showToast(`Group with ID ${id} already exists!`, "danger");
      return;
    }

    const newGroup = new Group(id, name, s1, s2);
    groups.push(newGroup);
    saveState();
    renderGroups();
    populateDropdowns();
    updateStats();

    showToast(`Group "${name}" created successfully!`, "success");
    logTerminal(`Created new Group: ID: ${id} | Name: ${name} | Required Skills: ${s1}, ${s2}`);

    e.target.reset();
    addGroupModal.classList.add("hidden");
  });

  // Send Join Request Modal Handlers
  const requestModal = document.getElementById("modal-send-request");
  document.getElementById("open-join-modal").addEventListener("click", () => {
    requestModal.classList.remove("hidden");
  });
  document.getElementById("close-request-modal").addEventListener("click", () => {
    requestModal.classList.add("hidden");
  });
  document.getElementById("cancel-request-modal").addEventListener("click", () => {
    requestModal.classList.add("hidden");
  });

  document.getElementById("form-send-request").addEventListener("submit", (e) => {
    e.preventDefault();
    const studentID = document.getElementById("req-student-select").value;
    const groupID = document.getElementById("req-group-select").value;

    enqueueJoinRequest(studentID, groupID);
    requestModal.classList.add("hidden");
  });

  // Reset Data Button
  document.getElementById("reset-data-btn").addEventListener("click", () => {
    if (confirm("Reset all students, groups, and queue back to the original C++ defaults?")) {
      loadDefaultState();
      renderGroups();
      renderStudents();
      renderQueue();
      updateStats();
      showToast("Data reset to initial defaults.", "info");
      logTerminal("=== Data reset to initial C++ defaults ===");
    }
  });

  // Clear Terminal Button
  document.getElementById("clear-terminal-btn").addEventListener("click", () => {
    document.getElementById("terminal-output").textContent = "";
  });
});
